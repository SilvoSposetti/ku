#include "DancingCellsStructure.h"

#include <algorithm>
#include <iostream>
#include <map>

std::pair<DancingCellsStructure, std::vector<OptionData>>
DancingCellsStructure::createStructure(const std::vector<std::vector<Sudo::Digit>>& board,
                                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

  int32_t primaryItemsCount = 0;
  int32_t secondaryItemsCount = 0;

  for (const auto& constraint : constraints) {
    primaryItemsCount += constraint->getPrimaryItemsAmount();
    secondaryItemsCount += constraint->getSecondaryItemsAmount();
  }

  // Count how many digits are given
  int32_t givensAmount = 0;
  for (const auto& row : board) {
    for (const auto& digit : row) {
      if (digit != Sudo::Digit::NONE) {
        ++givensAmount;
      }
    }
  }

  // At most 729 options => (9x9 grid,9 possible digits for each cell)
  constexpr int32_t maximumOptions = Sudo::MAX_DIGIT * Sudo::MAX_DIGIT * Sudo::MAX_DIGIT;
  // Each given digit reduces the amount of options by (Sudo::MAX_DIGIT - 1)
  const int32_t totalOptions = maximumOptions - (Sudo::MAX_DIGIT - 1) * givensAmount;

  // Data for the options
  auto optionsData = std::vector<OptionData>(totalOptions);

  auto optionsCache = std::vector<std::vector<XccElement>>(totalOptions);
  optionsCache.reserve(totalOptions);
  int32_t optionId = 0;
  int32_t overallOptionId = 0;
  for (const auto& boardI : Sudo::INDICES) {
    for (const auto& boardJ : Sudo::INDICES) {
      const Sudo::Digit actualDigit = board[boardI][boardJ];
      for (const auto& possibleDigit : Sudo::SUDO_DIGITS) {
        if (actualDigit == Sudo::Digit::NONE || actualDigit == possibleDigit) {
          int32_t baseItemId = 0;
          optionsData[optionId] = OptionData(boardI, boardJ, possibleDigit);
          for (const auto& constraint : constraints) {
            // Primary items
            const int32_t primaryItemsAmount = constraint->getPrimaryItemsAmount();
            if (primaryItemsAmount > 0) {
              const auto& primaryItems = constraint->getPrimaryItems();
              for (const auto& primaryItemId : primaryItems[overallOptionId]) {
                optionsCache[optionId].emplace_back(primaryItemId + baseItemId);
              }
              baseItemId += primaryItemsAmount;
            }
          }
          for (const auto& constraint : constraints) {
            // Secondary items
            const int32_t secondaryItemsAmount = constraint->getSecondaryItemsAmount();
            if (secondaryItemsAmount > 0) {
              const auto& secondaryItems = constraint->getSecondaryItems();
              for (const auto& secondaryItemId : secondaryItems[overallOptionId]) {
                optionsCache[optionId].emplace_back(secondaryItemId + baseItemId);
              }
              baseItemId += secondaryItemsAmount;
            }
          }
          optionId++;
        }
        overallOptionId++;
      }
    }
  }

  return {createStructure(primaryItemsCount, secondaryItemsCount, optionsCache), optionsData};
}

DancingCellsStructure DancingCellsStructure::createStructure(int32_t primaryItemsCount,
                                                             int32_t secondaryItemsCount,
                                                             const std::vector<std::vector<XccElement>>& options)

{
  int32_t itemsCount = primaryItemsCount + secondaryItemsCount;
  int32_t optionsCount = options.size();
  std::vector<int32_t> ITEM;
  std::vector<int32_t> SET;
  std::vector<DancingCellsNode> NODE;
  std::unordered_map<int32_t, int32_t> nodeIndicesToOptionIdMap;
  {
    // TODO: check if these preconditions can be removed/relaxed
    if (primaryItemsCount <= 0 || options.empty()) {
      throw std::runtime_error(std::string("No items or options provided"));
    }
  }

  // Element IDs are sorted in each option because XccOption uses sets
  {
    for (const auto& option : options) {
      if (!std::is_sorted(option.begin(), option.end())) {
        throw std::runtime_error(std::string("An option's element's IDs are not sorted"));
      }
    }
  }

  // Element IDs must be in the correct ranges for primary and secondary items
  {
    for (const auto& option : options) {
      // Note, the vector is sorted
      if (option.front().id < 0 || option.back().id >= optionsCount) {
        throw std::runtime_error(std::string("Invalid item ID"));
      }
    }
  }

  // Cannot be solved if any item cannot be covered
  {
    auto isPrimaryItemCoverable = std::vector<bool>(primaryItemsCount, false);
    for (const auto& option : options) {
      for (const auto& element : option) {
        isPrimaryItemCoverable[element.id] = true;
      }
    }
    if (std::any_of(isPrimaryItemCoverable.begin(), isPrimaryItemCoverable.end(), [](bool isCoverable) {
          return !isCoverable;
        })) {
      throw std::runtime_error(std::string("A primary item is cannot be covered with the given options"));
    }
  }

  constexpr int32_t undefinedColor = 0;

  // nodeCount, setCount, and setBlockCount
  // The total amount of nodes
  int32_t nodeCount = 0;

  auto setBlockCount = std::vector<int32_t>(itemsCount, 0);
  {
    for (const auto& option : options) {
      for (const auto& element : option) {
        setBlockCount[element.id]++;
      }
      nodeCount += option.size();
    }
    // Add spacer nodes at the beginning, end, and between each option
    nodeCount += options.size() + 1;
  }
  int32_t setCount = 0;
  {
    for (const auto& blockSize : setBlockCount) {
      // Size of the block, plus two elements before the block (pos, size)
      setCount += blockSize + 2;
    }
  }

  auto setBlockSums = std::vector<int32_t>(itemsCount, 0);
  {
    for (size_t i = 1; i < setBlockCount.size(); i++) {
      setBlockSums[i] = setBlockSums[i - 1] + setBlockCount[i - 1];
    }
  }

  std::map<int32_t, std::vector<int32_t>> map;
  const auto addToMap = [&](int32_t id, int32_t location) {
    if (map.contains(id)) {
      map[id].emplace_back(location);
    } else {
      map[id] = {location};
    }
  };

  // Start with SET's "pos" & "size" fields
  SET.resize(setCount, -1);
  {
    for (int32_t i = 0; i < setBlockCount.size(); i++) {
      const int32_t baseIndex = setBlockSums[i] + (i + 1) * 2;
      SET[baseIndex - 2] = i;
      SET[baseIndex - 1] = setBlockCount[i];
    }
  }

  // NODE & SET & ITEM
  auto itemCounters = std::vector<int32_t>(itemsCount, 0);
  ITEM = std::vector<int32_t>(itemsCount, -1);
  NODE.reserve(nodeCount);
  auto setBlockCounters = std::vector<int32_t>(itemsCount, 0);
  // First elment of NODE is a spacer
  NODE.emplace_back(0, options[0].size(), undefinedColor);
  int32_t optionIndex = 0;
  for (const auto& option : options) {
    for (const auto& element : option) {
      const int32_t itemIndex = element.id;
      const int32_t itemsBefore = itemIndex == 0 ? 0 : setBlockSums[itemIndex];
      const int32_t baseIndex = itemsBefore + (itemIndex + 1) * 2;
      const int32_t setIndex = baseIndex + setBlockCounters[itemIndex];
      const int32_t locIndex = baseIndex + itemCounters[itemIndex];
      SET[setIndex] = NODE.size();
      if (setBlockCounters[itemIndex] == 0) {
        ITEM[itemIndex] = setIndex;
      }
      setBlockCounters[itemIndex]++;

      // The first element in the option IDs map is right after the first spacer
      nodeIndicesToOptionIdMap.insert(std::make_pair(NODE.size(), optionIndex));
      // Actual node element
      addToMap(element.id, NODE.size());
      NODE.emplace_back(baseIndex, locIndex, element.colorId);
      itemCounters[itemIndex]++;
    }
    const bool lastOption = optionIndex >= options.size() - 1;
    optionIndex++;

    if (!lastOption) {
      // Regular spacer
      const auto& nextOption = options[optionIndex];
      NODE.emplace_back(-option.size(), nextOption.size(), undefinedColor);
    } else {
      // Last spacer
      NODE.emplace_back(-option.size(), 0, undefinedColor);
    }
  }

  return DancingCellsStructure{.ITEM = ITEM,
                               .SET = SET,
                               .NODE = NODE,
                               .primaryItemsCount = primaryItemsCount,
                               .secondaryItemsCount = secondaryItemsCount,
                               .itemsCount = itemsCount,
                               .optionsCount = optionsCount,
                               .nodeIndicesToOptionIdMap = nodeIndicesToOptionIdMap};
}

void DancingCellsStructure::print() const {

  std::cout << "ITEM" << std::endl;
  for (const auto& i : ITEM) {
    std::cout << i << ", ";
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "SET" << std::endl;
  int32_t i = 0;
  for (const auto& element : SET) {
    std::cout << i << " - " << element << std::endl;
    i++;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "NODE" << std::endl;
  int32_t j = 0;
  for (const auto& element : NODE) {
    if (element.item <= 0) {
      std::cout << " " << std::endl;
    }
    std::cout << j << " - \tITM:" << element.item << "\tLOC:" << element.location << "\tCLR" << element.color << ""
              << std::endl;
    if (element.item <= 0) {
      std::cout << " " << std::endl;
    }
    j++;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "optionsMap" << std::endl;
  for (const auto& [firstNodeOfOptionIndex, optionIndex] : nodeIndicesToOptionIdMap) {

    std::cout << "nodeIndex: " << firstNodeOfOptionIndex << "\t - optionIndex: " << optionIndex << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
}

int32_t& DancingCellsStructure::pos(int32_t index) {
  return SET[index - 2];
}

int32_t& DancingCellsStructure::size(int32_t index) {
  return SET[index - 1];
}
