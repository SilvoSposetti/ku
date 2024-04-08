#include "DataStructure.h"

#include "../Sudo.h"
#include "Node.h"

#include <algorithm>

DataStructure::DataStructure(const std::vector<std::vector<Sudo::Digit>>& board,
                             const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

  int32_t nonZeroElementsAmount = 0;
  std::vector<std::vector<int32_t>> optionsCache;
  {
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

    // Fill items data
    {
      // Count how many items are required
      int32_t totalItems = 0;
      for (const auto& constraint : constraints) {
        totalItems += constraint->getPrimaryItemsAmount();
        totalItems += constraint->getSecondaryItemsAmount();
      }
      itemsData = std::vector<ItemData>(totalItems);

      int32_t itemCounter = 0;
      // PrimaryItems
      for (const auto& constraint : constraints) {
        const int32_t itemsAmount = constraint->getPrimaryItemsAmount();
        if (itemsAmount > 0) {
          const std::string constraintName = constraint->getName();
          for (int32_t itemId = 0; itemId < itemsAmount; itemId++) {
            itemsData[itemCounter] = ItemData(constraintName, true, itemId);
            itemCounter++;
          }
        }
      }
      // SecondaryItems
      for (const auto& constraint : constraints) {
        const int32_t itemsAmount = constraint->getSecondaryItemsAmount();
        if (itemsAmount > 0) {
          const std::string constraintName = constraint->getName();
          for (int32_t itemId = 0; itemId < itemsAmount; itemId++) {
            itemsData[itemCounter] = ItemData(constraintName, false, itemId);
            itemCounter++;
          }
        }
      }
    }

    // Data for the options
    optionsData = std::vector<OptionData>(totalOptions);

    // Count how many non-zero elements there are in the hypothetical matrix
    optionsCache = std::vector<std::vector<int32_t>>(totalOptions, std::vector<int32_t>());
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
                nonZeroElementsAmount += primaryItems[overallOptionId].size();
                for (const auto& primaryItemId : primaryItems[overallOptionId]) {
                  optionsCache[optionId].emplace_back(primaryItemId + baseItemId);
                }
                baseItemId += primaryItemsAmount;
              }
              // Secondary items
              const int32_t secondaryItemsAmount = constraint->getSecondaryItemsAmount();
              if (secondaryItemsAmount > 0) {
                const auto& secondaryItems = constraint->getSecondaryItems();
                nonZeroElementsAmount += secondaryItems[overallOptionId].size();
                for (const auto& itemId : secondaryItems[overallOptionId]) {
                  optionsCache[optionId].emplace_back(itemId + baseItemId);
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
  }

  structure = createStructure(itemsData, optionsData, optionsCache, nonZeroElementsAmount);
}

DataStructure::DataStructure(const std::vector<std::vector<bool>>& matrix, int32_t secondaryItemsAmount) {
  int32_t nonZeroElementsAmount = 0;
  std::vector<std::vector<int32_t>> optionsCache;
  {
    if (matrix.empty() || matrix[0].empty()) {
      return;
    }
    const int32_t itemsAmount = matrix[0].size();
    const int32_t optionsAmount = matrix.size();
    for (const auto& row : matrix) {
      nonZeroElementsAmount += std::count(row.begin(), row.end(), true);
    }

    // Matrix should contain something
    if (itemsAmount == 0 || optionsAmount == 0 || nonZeroElementsAmount == 0) {
      return;
    }

    // Matrix's rows should all be the same size
    if (std::any_of(matrix.begin(), matrix.end(), [&](const std::vector<bool> row) {
          return static_cast<int32_t>(row.size()) != itemsAmount;
        })) {
      return;
    }

    // Initialize itemsData
    itemsData = std::vector<ItemData>(itemsAmount);
    const int32_t primaryItemsAmount = itemsAmount - secondaryItemsAmount;
    for (int32_t primaryItemId = 0; primaryItemId < primaryItemsAmount; primaryItemId++) {
      itemsData[primaryItemId] = ItemData(std::to_string(primaryItemId), true, primaryItemId);
    }
    for (int32_t secondaryItemId = 0; secondaryItemId < secondaryItemsAmount; secondaryItemId++) {
      itemsData[primaryItemsAmount + secondaryItemId] =
          ItemData(std::to_string(secondaryItemId), false, secondaryItemId);
    }

    // Initialize optionsData
    optionsData = std::vector<OptionData>(optionsAmount);
    for (int32_t optionId = 0; optionId < optionsAmount; optionId++) {
      optionsData[optionId] = OptionData(optionId, 0, Sudo::Digit::NONE);
    }

    // Create options cache
    optionsCache = std::vector<std::vector<int32_t>>(optionsAmount, std::vector<int32_t>());
    for (int32_t optionId = 0; optionId < optionsAmount; optionId++) {
      for (int32_t itemId = 0; itemId < itemsAmount; itemId++) {
        if (matrix[optionId][itemId]) {
          optionsCache[optionId].emplace_back(itemId);
        }
      }
    }
  }

  structure = createStructure(itemsData, optionsData, optionsCache, nonZeroElementsAmount);
}

std::vector<Node> DataStructure::createStructure(const std::vector<ItemData>& items,
                                                 const std::vector<OptionData>& options,
                                                 const std::vector<std::vector<int32_t>>& optionsCache,
                                                 int32_t nonZeroElementsAmount) {

  // Nodes contain "pointers" to other elements in the structure in the form of integer indices.
  // The structure is composed of multiple sections packed into a single vector of Nodes.
  // 1. A root node.
  // 2. A list of header nodes. A header is given to every item.
  //    - Header nodes make a circular doubly linked-list together with the root node.
  //    - Header nodes have a length attribute that keeps track of how many elements are contained within the item.
  // 3. A list of all the valid spare coordinate matrix entries in row-major order. Different rows are separated by
  //    spacer nodes. These spacer nodes appear at both ends of these entries as well.
  //    And additionally:
  //    - Every node of the sparse coordinate matrix composes a 2-dimensional doubly-linked list because it holds
  //    pointers to its left-right- and to its up-down-neighbors in the original sparse matrix.
  //    - Every node also holds a pointer to its header.
  // In particular, a spacer node N has:
  // - An up-link to the first node in the option before N.
  // - An down-link to the last node in the option after N.

  // One root, plus as many headers as there are items, plus one regular node for each non-zero element, plus one spacer
  // in fron to each option, plus one spacer at the end
  const int32_t totalNodesAmount = 1 + items.size() + nonZeroElementsAmount + options.size() + 1;
  // Initialize structure with correct size
  std::vector<Node> structure = std::vector<Node>(totalNodesAmount);

  // Set the root node
  structure[0].type = NodeType::Root;

  // I1: Read the first line
  int32_t N_1 = -1;
  int32_t i = 0;

  for (const auto& item : items) {
    i++;
    structure[i].type = NodeType::Header;
    // structure[i].name = std::to_string(itemId);
    structure[i].left = i - 1;
    structure[i - 1].right = i;
    if (!item.isPrimary && N_1 <= 0) {
      N_1 = i - 1;
    }
  }

  // I2:  Finish the horizontal list
  int32_t N = i;
  if (N_1 <= 0) {
    // There were no secondary items
    N_1 = N;
  }
  // The active secondary items (if any) are accessible  the node at index N+1
  structure[N + 1].left = N;
  structure[N].right = N + 1;
  structure[N_1 + 1].left = N + 1;
  structure[N + 1].right = N_1 + 1;
  structure[0].left = N_1;
  structure[N_1].right = 0;

  // I3: Prepare for options
  for (int32_t j = 1; j <= N; j++) {
    structure[j].length = 0;
    structure[j].up = j;
    structure[j].down = j;
  }
  int32_t M = 0;
  int32_t p = N + 1;
  // At this moment, p is the first spacer
  structure[p].type = NodeType::Spacer;
  structure[p].header = -1;

  for (size_t optionId = 0; optionId < options.size(); optionId++) {
    int32_t j = 0;
    for (const auto& i_j_index : optionsCache[optionId]) {
      const int32_t i_j = i_j_index + 1;
      j++;
      // I4: Read an option
      structure[p + j].type = NodeType::Node;
      structure[i_j].length++;
      // p + j is the index of the current node, just added to the structure
      const int32_t q = structure[i_j].up; // q is the bottom-est node under header i_j
      structure[q].down = p + j; // Set the down of q to the current
      structure[p + j].up = q; // Set q as the top node of the current
      structure[p + j].down = i_j; // Set the down of the current node, which is now the bottom-est
      structure[p + j].header = i_j; // Set the header of the current node
      structure[i_j].up = p + j; // Set the up of the header to the
    }
    // I5: Finish an option
    const int32_t k = j;
    M = M + 1;
    structure[p].down = p + k;
    p = p + k + 1;
    structure[p].type = NodeType::Spacer;
    structure[p].header = -M;
    structure[p].up = p - k;
  }

  // structure is now complete
  return structure;
}

std::vector<Node> DataStructure::getStructureCopy() const {
  return structure;
}

int32_t DataStructure::getItemsAmount() const {
  return itemsData.size();
}

const std::vector<ItemData>& DataStructure::getItemsData() const {
  return itemsData;
}

int32_t DataStructure::getOptionsAmount() const {
  return optionsData.size();
}

const std::vector<OptionData>& DataStructure::getOptionsData() const {
  return optionsData;
}

bool DataStructure::isSolvableByAlgorithmX() const {
  return true;
}

void DataStructure::print() const {

  // Helper function
  const auto getInfo = [&](const std::string& inName, int32_t value) -> std::string {
    if (value >= 0) {
      std::string valueString = std::to_string(value);
      while (valueString.size() < 2) {
        valueString = " " + valueString;
      }
      return inName + ": " + valueString + "   ";
    }
    return "";
  };

  int32_t counter = 0;
  for (const auto& node : structure) {
    std::string s;
    s += getInfo("ID", counter);
    s += node.getTypeString() + "  ";
    s += getInfo("H", node.header);
    s += getInfo("U", node.up);
    s += getInfo("D", node.down);
    s += getInfo("L", node.left);
    s += getInfo("R", node.right);
    s += getInfo("LEN", node.length);
    std::cout << s << std::endl;
    counter++;
  }
  std::cout << std::endl;
}
