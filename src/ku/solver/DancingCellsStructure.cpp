#include "DancingCellsStructure.h"

#include <algorithm>
#include <iostream>

DancingCellsStructure::DancingCellsStructure(const std::vector<std::vector<Sudo::Digit>>& board,
                                             const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

  // Set up primary- secondary- and itemsCount, as well as optionsCount
  {
    int32_t primaryItemsCount = 0;
    int32_t secondaryItemsCount = 0;
    int32_t nodesCount = 0;
    for (const auto& constraint : constraints) {
      primaryItemsCount += constraint->getPrimaryItemsAmount();
      secondaryItemsCount += constraint->getSecondaryItemsAmount();
      nodesCount += constraint->getPrimaryItems().size();
      nodesCount += constraint->getSecondaryItems().size();
    }
    itemsCount = primaryItemsCount + secondaryItemsCount;
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
    int32_t optionsCount = maximumOptions - (Sudo::MAX_DIGIT - 1) * givensAmount;
    intializeMembers(primaryItemsCount, secondaryItemsCount, optionsCount, nodesCount);
  }

  // Phase 1:
  int32_t last_node = 0; // The first node in NODE that's not yet used
  int32_t second = secondaryItemsCount > 0 ? primaryItemsCount + 1 : std::numeric_limits<int32_t>::max();

  // Data for the options
  optionsData = std::vector<OptionData>(optionsCount);

  int32_t optionId = 0;
  int32_t overallOptionId = 0;
  for (const auto& boardI : Sudo::INDICES) {
    for (const auto& boardJ : Sudo::INDICES) {
      const Sudo::Digit actualDigit = board[boardI][boardJ];
      for (const auto& possibleDigit : Sudo::SUDO_DIGITS) {
        if (actualDigit == Sudo::Digit::NONE || actualDigit == possibleDigit) {
          int32_t baseItemId = 0;
          //
          int32_t i = last_node; // Remember the spacer before this option in NODE;
          //
          optionsData[optionId] = OptionData(boardI, boardJ, possibleDigit);
          for (const auto& constraint : constraints) {
            // Primary items
            const int32_t primaryItemsAmount = constraint->getPrimaryItemsAmount();
            if (primaryItemsAmount > 0) {
              const auto& primaryItems = constraint->getPrimaryItems();
              for (const auto& primaryItemId : primaryItems[overallOptionId]) {
                int32_t itemId = primaryItemId + baseItemId;
                createNodeForItem({itemId}, last_node, optionId);
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
                int32_t itemId = secondaryItemId + baseItemId;
                createNodeForItem({itemId}, last_node, optionId);
              }
              baseItemId += secondaryItemsAmount;
            }
          }
          //
          NODE[i].location = last_node - i; // Complete the previous spacer
          last_node++; // Create the next spacer
          NODE[last_node].item = i + 1 - last_node;
          //
          optionId++;
        }
        overallOptionId++;
      }
    }
  }

  finishInitialization(second, last_node);
}

DancingCellsStructure::DancingCellsStructure(int32_t primaryItemsCount,
                                             int32_t secondaryItemsCount,
                                             const std::vector<std::vector<XccElement>>& options) {
  checkOptions(options, primaryItemsCount, primaryItemsCount + secondaryItemsCount);

  int32_t nodesCount = 0;
  for (const auto& option : options) {
    nodesCount += option.size();
  }

  intializeMembers(primaryItemsCount, secondaryItemsCount, options.size(), nodesCount);

  int32_t last_node = 0; // The first node in NODE that's not yet used
  int32_t second = secondaryItemsCount > 0 ? primaryItemsCount + 1 : std::numeric_limits<int32_t>::max();

  // Take advantage of the fact that SET will always be larger than NODE. Therefore partially complete NODE and use
  // SET's memory as temporary storage for data that will be useful in the next phases.

  // TODO: remove Phases
  // Phase 1: Go through all the options and gather as much information as possible
  int32_t optionIndex = 0;
  for (const auto& option : options) {
    int32_t i = last_node; // Remember the spacer before this option in NODE;
    if (!option.empty()) {
      for (const auto& element : option) {
        createNodeForItem(element, last_node, optionIndex);
      }
      NODE[i].location = last_node - i; // Complete the previous spacer
      last_node++; // Create the next spacer
      NODE[last_node].item = i + 1 - last_node;
    }
    optionIndex++;
  }

  // Now, after Phase 1,
  // NODE contains:
  // - Already completed spacers, whose attributes are:
  //   - 'item' contains negative the amount of nodes in the option before the spacer (or 0 for the first spacer)
  //   - 'location' contains the amount of nodes in the option after the spacer (or 0 for the last spacer)
  //   - 'color' unused
  // - Regular nodes are filled, but with attributes helpful in the next phase:
  //   - 'item' contains the 1-based index of the item for this option
  //   - 'location'contains the amount of nodes of the same item that were found when this node was inserted
  //   - 'color' attributes
  //
  // SET contains:
  // pair of 'last_node' and 'size' values, where size is already the final value of size(k) for an item k, only in a
  // different location (except for the very first pair).
  // Therefore, SET contains data in contiguous pairs like this:
  // [(last_node, size), (last_node, size), ...
  //   item index 0       item index 1

  finishInitialization(second, last_node);

  // print();
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
    std::cout << j << " - \titem:" << element.item << "\tlocation:" << element.location << "\tcolor:" << element.color
              << "" << std::endl;
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

void DancingCellsStructure::checkOptions(const std::vector<std::vector<XccElement>>& options,
                                         int32_t primaryItemsCount,
                                         int32_t itemsCount) {
  {
    // TODO: check if these preconditions can be removed/relaxed
    if (primaryItemsCount <= 0 || options.empty()) {
      throw std::runtime_error(std::string("No items or options provided"));
    }
  }

  {
    auto areItemsCoverable = std::vector<bool>(itemsCount, false);
    for (const auto& option : options) {
      // Every option must list item IDs in order
      if (!std::is_sorted(option.begin(), option.end())) {
        throw std::runtime_error(std::string("An option's element's IDs are not sorted"));
      }
      // Option IDs must be in the valid range. Only need to check first and last item since the option vector is sorted
      if (!option.empty() && (option.front().id < 0 || option.back().id >= itemsCount)) {
        throw std::runtime_error(std::string("Invalid item ID"));
      }
      for (const auto& element : option) {
        areItemsCoverable[element.id] = true;
      }
    }
    if (std::any_of(
            areItemsCoverable.begin(), areItemsCoverable.end(), [](bool isCoverable) { return !isCoverable; })) {
      throw std::runtime_error(std::string("A primary item is cannot be covered with the given options"));
    }
  }
}

void DancingCellsStructure::intializeMembers(int32_t primaryItemsCount,
                                             int32_t secondaryItemsCount,
                                             int32_t optionsCount,
                                             int32_t nodesCount) {

  this->primaryItemsCount = primaryItemsCount;
  this->secondaryItemsCount = secondaryItemsCount;
  itemsCount = primaryItemsCount + secondaryItemsCount;
  this->optionsCount = optionsCount;

  // TODO: do these vector really need to be initialized to zero?
  ITEM = std::vector<int32_t>(itemsCount, 0);
  const int32_t setPosAndSizeCellsCount = itemsCount * 2;
  SET = std::vector<int32_t>(nodesCount + setPosAndSizeCellsCount, 0);
  const int32_t nodeSpacersCount = optionsCount + 1;
  NODE = std::vector<DancingCellsNode>(nodesCount + nodeSpacersCount, DancingCellsNode(0, 0, 0));
  // TODO: Can the map be a vector? (nodeindex is literally the index of a vector element)
}

void DancingCellsStructure::finishInitialization(int32_t second, int32_t last_node) {
  // Phase 2: Set every elemnt into ITEM
  // After Phase 2, ITEM contains the correct values.
  {
    int32_t k = 0;
    // j is an inde x in SET
    int32_t j = 2; // The first node in ITEM will always be in position 2
    // k is an index in ITEM
    for (k = 0; k < itemsCount; k++) {
      ITEM[k] = j;
      // Add to j the size() of the node, going forward in every pair
      j += 2 + SET[((k + 1) << 1) - 1]; // TODO: replace with size()
    }

    // Update second
    if (second == std::numeric_limits<int32_t>::max()) { // there were secondary items
      second = j;
    }
  }

  // Phase 3: Expand SET, going from last element to first, move an item's pos() and size() values from the pair's
  // location, to where they should be.
  // After Phase 3, SET has now size() and pos() elements set correctly, there other nodes may still contain other, now
  // unused, values from before.
  {
    for (int32_t k = itemsCount; k > 0; k--) {

      // j is the index of item k into SET, where k is the 1-based index of an item.
      int32_t j = ITEM[k - 1];
      if (k == second) {
        second = j; // second is now an index into SET
      }
      SET[j - 1] = SET[(k << 1) - 1]; // TODO: replace with size() (twice)
      SET[j - 2] = k - 1; // TODO: replace with pos()
    }
  }

  // Phase 4: Adjust NODE
  {
    for (int32_t k = 1; k < last_node; k++) {
      if (NODE[k].item < 0) {
        continue;
      }
      int32_t j = ITEM[NODE[k].item - 1];
      int32_t i = j + NODE[k].location;
      NODE[k].item = j;
      NODE[k].location = i;
      SET[i] = k;
    }
  }
}

void DancingCellsStructure::createNodeForItem(const XccElement& element, int32_t& last_node, int32_t optionIndex) {

  {
    // create a node for the item

    // k points correctly one element after the temporary blocks of pairs (pos, size)
    int32_t k = (element.id + 1) * 2;
    last_node++;

    // t is how many previous options have used this item
    int32_t t = SET[k - 1]; // TODO: replace with size()
    NODE[last_node].item = k >> 1; // need to divide by two to get the 1-based index of the item
    NODE[last_node].location = t; // The amount of nodes for this item that were found until now
    NODE[last_node].color = element.colorId; // Set the correct color ID
    nodeIndicesToOptionIdMap.insert(std::make_pair(last_node, optionIndex));
    // Increase the amount of nodes are found for item index k
    SET[k - 1] = t + 1; // TODO: replace with size()
    // Set the index of the last node in NODE where itemm k has appeared
    SET[k - 2] = last_node; // TODO: replace with pos()
  }
}