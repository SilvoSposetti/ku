#include "DancingCellsStructure.hpp"

#include <algorithm>
#include <iostream>
#include <limits>

DancingCellsStructure::DancingCellsStructure(int32_t primaryItemsCount,
                                             int32_t secondaryItemsCount,
                                             const std::vector<std::vector<XccElement>>& options) {
  checkOptions(options, primaryItemsCount, primaryItemsCount + secondaryItemsCount);

  int32_t nodesCount = 0;
  for (const auto& option : options) {
    nodesCount += option.size();
  }

  allocateMemoryForMembers(primaryItemsCount, secondaryItemsCount, options.size(), nodesCount);

  int32_t lastNode = 0; // The first node in NODE that's not yet used
  int32_t second = secondaryItemsCount > 0 ? primaryItemsCount + 1 : std::numeric_limits<int32_t>::max();

  // Take advantage of the fact that SET will always be larger than NODE. Therefore partially complete NODE and use
  // SET's memory as temporary storage for data that will be useful whe finishing initialization.

  // Prepare NODE by going through all the options and gather as much information as possible
  int32_t optionIndex = 0;
  for (const auto& option : options) {
    int32_t previousSpacerIndex = lastNode; // Remember the spacer before this option in NODE;
    if (!option.empty()) {
      for (const auto& element : option) {
        createNodeForItem(element, lastNode, optionIndex);
      }
      NODE[previousSpacerIndex].location = lastNode - previousSpacerIndex; // Complete the previous spacer
      lastNode++; // Create the next spacer
      NODE[lastNode].item = previousSpacerIndex + 1 - lastNode;
    }
    optionIndex++;
  }

  finishInitialization(second, lastNode);
}

void DancingCellsStructure::checkOptions(const std::vector<std::vector<XccElement>>& options,
                                         int32_t primaryItemsCount,
                                         int32_t itemsCount) {

  if (primaryItemsCount <= 0 || options.empty()) {
    throw std::runtime_error(std::string("No primary items or options provided"));
  }

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
  if (std::any_of(areItemsCoverable.begin(), areItemsCoverable.end(), [](bool isCoverable) { return !isCoverable; })) {
    throw std::runtime_error(std::string("An item cannot be covered with the given options"));
  }
}

void DancingCellsStructure::allocateMemoryForMembers(int32_t primaryCount,
                                                     int32_t secondaryCount,
                                                     int32_t optionsCount,
                                                     int32_t nodesCount) {

  this->primaryItemsCount = primaryCount;
  this->secondaryItemsCount = secondaryCount;
  itemsCount = primaryItemsCount + secondaryItemsCount;
  this->optionsCount = optionsCount;

  ITEM = std::vector<int32_t>(itemsCount, 0);
  const int32_t setPosAndSizeCellsCount = itemsCount * 2;
  SET = std::vector<int32_t>(nodesCount + setPosAndSizeCellsCount, 0);
  const int32_t nodeSpacersCount = optionsCount + 1;
  NODE = std::vector<DancingCellsNode>(nodesCount + nodeSpacersCount, DancingCellsNode(0, 0, 0));
  nodeOptionIndices.resize(nodesCount + nodeSpacersCount);
}

void DancingCellsStructure::createNodeForItem(const XccElement& element, int32_t& lastNode, int32_t optionIndex) {
  // baseSetIndex is the index in SET to one element after the temporary blocks of pairs (pos, size)
  // I.e. it points to the first element of the next pair, because of how size() and pos() work.
  int32_t baseSetIndex = (element.id + 1) * 2;
  lastNode++;

  int32_t t = size(baseSetIndex); // How many previous options have used this item
  NODE[lastNode].item = baseSetIndex >> 1; // Need to divide by two to get the 1-based index of the item
  NODE[lastNode].location = t; // The amount of nodes for this item that were found until now
  NODE[lastNode].color = element.colorId; // Set the correct color ID

  // Now that it's known where the elment landed in NODE, the map can be updated
  nodeOptionIndices[lastNode] = optionIndex;

  size(baseSetIndex) = t + 1; // Increase the amount of nodes are found for item index baseSetIndex
  position(baseSetIndex) = lastNode; // Set the index of the last node in NODE where item baseSetIndex has appeared
}

void DancingCellsStructure::finishInitialization(int32_t second, int32_t lastNode) {
  // Now, after going through all the options, the following explains the contents of NODE and SET.
  // NODE contains:
  // - Already completed spacers, whose attributes are:
  //   - 'item' contains negative the amount of nodes in the option before the spacer (or 0 for the first spacer)
  //   - 'location' contains the amount of nodes in the option after the spacer (or 0 for the last spacer)
  //   - 'color' unused
  // - Regular nodes are filled, but with attributes helpful in the next steps:
  //   - 'item' contains the 1-based index of the item for this option
  //   - 'location'contains the amount of nodes of the same item that were found when this node was inserted
  //   - 'color' attributes
  //
  // SET contains:
  // pair of 'lastNode' and 'size' values, where size is already the final value of size(k) for an item k, only in a
  // different location (except for the very first pair).
  // Therefore, SET contains data in contiguous pairs like this:
  // [(lastNode, size), (lastNode, size), ...
  //   item index 0       item index 1

  {
    // Set every element into ITEM
    int32_t itemIndex = 0;
    int32_t setIndex = 2; // The first node in ITEM will always be in position 2
    for (itemIndex = 0; itemIndex < itemsCount; itemIndex++) {
      ITEM[itemIndex] = setIndex;
      // Add to setIndex the size() of the node, going forward in every pair
      setIndex += 2 + size(((itemIndex + 1) << 1));
    }

    // Update second
    if (second == std::numeric_limits<int32_t>::max()) { // there were secondary items
      second = setIndex;
    }
  }
  // Now ITEM contains the correct values.

  {
    // Next step: Expand SET, going from last element to first, move an item's pos() and size() values from the pair's
    // location, to where they should be.
    // After Phase 3,
    for (int32_t itemIndexOneBased = itemsCount; itemIndexOneBased > 0; itemIndexOneBased--) {

      // setIndex is the index of the item into SET, where itemIndexOneBased is the 1-based index of an item.
      int32_t setIndex = ITEM[itemIndexOneBased - 1];
      if (itemIndexOneBased == second) {
        second = setIndex; // second is now an index into SET
      }
      size(setIndex) = size(itemIndexOneBased << 1);
      position(setIndex) = itemIndexOneBased - 1;
    }
  }
  // Now SET has  size() and pos() elements set correctly, its other cells nodes may still contain other, now unused,
  // values from before.

  {
    // Next step: Adjust NODE and SET with the remaining information
    for (int32_t nodeIndex = 1; nodeIndex < lastNode; nodeIndex++) {
      if (NODE[nodeIndex].item < 0) {
        // nodeIndex points to a spacer
        continue;
      }
      int32_t baseSetIndex = ITEM[NODE[nodeIndex].item - 1];
      int32_t setIndex = baseSetIndex + NODE[nodeIndex].location;
      NODE[nodeIndex].item = baseSetIndex;
      NODE[nodeIndex].location = setIndex;
      SET[setIndex] = nodeIndex;
    }
  }
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
  int32_t nodeIndex = 0;
  for (const auto& element : NODE) {
    if (element.item <= 0) {
      std::cout << " " << std::endl;
    }
    std::cout << nodeIndex << " - \titem:" << element.item << "\tlocation:" << element.location
              << "\tcolor:" << element.color << "" << std::endl;
    if (element.item <= 0) {
      std::cout << " " << std::endl;
    }
    nodeIndex++;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "optionsMap" << std::endl;
  for (size_t i = 0; i < nodeOptionIndices.size(); i++) {
    std::cout << "nodeIndex: " << i << "\t - optionIndex: " << nodeOptionIndices[i] << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
}
