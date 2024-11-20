#include "AlgorithmC.h"

#include "../randomGenerator/RandomGenerator.h"

#include <limits>

/** Helper functions for Algorithm C have been put here in an unnamed namespace such that they are only accessible by
 * this translation unit and nowhere else
 */
namespace {

/** Selects one the of the items with smallest length in the active list of the data structure. If there are
 * multiple items with the same smallest length, it selects one of those at random.
 * @param structure A reference to the structure currently being used
 * @param randomGenerator A reference to the random number generator being used
 * @param smallestSizeFoundSoFar A reference to the smallest size found so far, enter this function as the maximum
 * int32_t allowed.
 * @param bestItemIndex The item in the datastructure that has been picked by the MRV heuristic
 * @param active The amount of active items in the structure
 * @param second The index of the first secondary item in the structure
F */
void pickItemUsingMrvHeuristic(DancingCellsStructure& structure,
                               RandomGenerator& randomGenerator,
                               int32_t& smallestSizeFoundSoFar,
                               int32_t& bestItemIndex,
                               int32_t active,
                               int32_t second) {
  const auto selectThisItem = [&](int32_t itemIndex, int32_t sizeOfItem) {
    bestItemIndex = structure.ITEM[itemIndex];
    smallestSizeFoundSoFar = sizeOfItem;
  };

  int32_t smallestSizeNodesAmount = 0;
  for (int32_t itemIndex = 0; smallestSizeFoundSoFar > 1 && itemIndex < active; itemIndex++) {
    if (structure.ITEM[itemIndex] < second) {
      int32_t sizeOfItem = structure.size(structure.ITEM[itemIndex]);
      if (sizeOfItem < smallestSizeFoundSoFar) {
        // New size is smaller than the smallest one found so far, select it
        selectThisItem(itemIndex, sizeOfItem);
        smallestSizeNodesAmount = 1;
      } else if (sizeOfItem == smallestSizeFoundSoFar) {
        // New size is equal to the smallest one found so far, randomly select it.
        smallestSizeNodesAmount++;
        // Chance to select this item is: one over the amount of items found so far with this smallest size.
        // This ensures that every item with smallest size has an equal probability of being selected.
        const bool needToSelectThisItem =
            randomGenerator.uniformFloat(0.0f, 1.0f) < (1.0f / static_cast<float>(smallestSizeNodesAmount));
        if (needToSelectThisItem) {
          selectThisItem(itemIndex, sizeOfItem);
        }
      }
    }
  }
};

/** Computes sets of option indices that solve the problem described by the data structure
 * @param structure The structure that was used by Algorithm C
 * @param nodeIndicesSets The sets of node indices that describe the Exact Covering with Colors problem's solutions
 */
std::vector<std::unordered_set<int32_t>>
retrieveOptionIndices(const DancingCellsStructure& structure,
                      const std::vector<std::unordered_set<int32_t>>& nodeIndicesSets) {

  std::vector<std::unordered_set<int32_t>> solutions;
  solutions.reserve(nodeIndicesSets.size());
  for (const auto& nodeIndicesSet : nodeIndicesSets) {
    std::unordered_set<int32_t> solution;
    for (const auto value : nodeIndicesSet) {
      solution.insert(structure.nodeOptionIndices.at(value));
    }
    solutions.push_back(solution);
  }
  return solutions;
};

/** Hide all of the incompatible options remaining in the set of a given item.
 * If check is true, this function returns zero if that would cause a primary item to be uncoverable.
 * @param structure A reference to the structure
 * @param setBaseIndex The base index of the item in SET that needs to hide incompatible options
 * @param color The color information for the item that's being hidden
 * @param performEarlyExitIfPrimaryItemIsUncoverable Whether to
 * @param previousActive
 * @param second
 * @param active
 * @return Whether hiding the incompatible options is successful
 */
bool hide(DancingCellsStructure& structure,
          int32_t setBaseIndex,
          int32_t color,
          bool performEarlyExitIfPrimaryItemIsUncoverable,
          int32_t previousActive,
          int32_t second,
          int32_t active) {
  int32_t setIndex = setBaseIndex;
  int32_t size = setBaseIndex + structure.size(setBaseIndex);

  for (; setIndex < size; setIndex++) {
    int32_t nodeIndex = structure.SET[setIndex];
    const bool isColorUndefined = !color;
    const bool doesNodeHaveDifferentColor = structure.NODE[nodeIndex].color != color;
    if (isColorUndefined || doesNodeHaveDifferentColor) {
      //  remove option tt from the other sets it's in
      {
        for (int32_t siblingNodeIndex = nodeIndex + 1; siblingNodeIndex != nodeIndex;) {
          int32_t siblingNodeItem = structure.NODE[siblingNodeIndex].item;
          if (siblingNodeItem < 0) {
            // This node is a spacer, loop back to the index in NODE that represents the first item in the option
            siblingNodeIndex += siblingNodeItem;
            continue;
          }

          if (structure.position(siblingNodeItem) < previousActive) {
            // If the sibling node's item is active
            int32_t newSize = structure.size(siblingNodeItem) - 1;
            const bool isLastNodeOfSiblingItem = newSize == 0;
            const bool isSiblingItemPrimary = siblingNodeItem < second;
            const bool isSiblingItemActive = structure.position(siblingNodeItem) < active;
            if (performEarlyExitIfPrimaryItemIsUncoverable // Early return is enabled
                && isSiblingItemPrimary // The sibling item is primary
                && isLastNodeOfSiblingItem // The sibling item has only one option left
                && isSiblingItemActive // The sibling item active
            ) {
              // Then there's no way that the item can be covered.
              // Hiding is not successful, -> early exit and report
              return false;
            }

            // Proceed with hiding the node of the sibling item
            int32_t newNodeLocation = structure.SET[siblingNodeItem + newSize];
            int32_t siblingNodeLocation = structure.NODE[siblingNodeIndex].location;
            structure.size(siblingNodeItem) = newSize;
            structure.SET[siblingNodeItem + newSize] = siblingNodeIndex;
            structure.SET[siblingNodeLocation] = newNodeLocation;
            structure.NODE[siblingNodeIndex].location = siblingNodeItem + newSize;
            structure.NODE[newNodeLocation].location = siblingNodeLocation;
          }
          siblingNodeIndex++;
        }
      }
    }
  }
  // Hiding was successful
  return true;
}

/** Runs Algorithm C on the internal data structure to find a solution.
 * @param findFirstSolution Whether a single solution suffices. When true this offers early exit as soon as the first
 * one is found
 * @param checkForUniqueness Whether confirming a unique solution is required. When true this offers early exit
 * if more than one solution is found.
 * @param dataStructure The sparse matrix that defines an Exact Cover problem
 * @return One or multiple sets of indices pointing to nodes in the data structure. The elements of these sets point
 * to the first nodes of options that are part of the solution found.
 */
std::vector<std::unordered_set<int32_t>> runAlgorithmC(DancingCellsStructure& dataStructure,
                                                       bool findFirstSolution,
                                                       bool checkForUniqueness,
                                                       const std::optional<int32_t>& seed) {

  // Don't run Algorithm C on an empty structure
  if (dataStructure.optionsCount == 0) {
    return {};
  }

  // Get a copy of the structure to work on
  auto structure = dataStructure;

  // The following vector will store the solutions, i.e. the sets of nodes which are contained by the options that
  // solve the exact cover problem
  std::vector<std::unordered_set<int32_t>> solutions;

  // Initialize random generator to pick columns randomly according to a certain seed
  RandomGenerator randomGenerator(seed);

  constexpr int32_t maxInt = std::numeric_limits<int32_t>::max();

  // The currently active items are the ones to the left of the "active" index in the ITEM list.
  int32_t active = structure.itemsCount;
  // Second is the internal number of the smallest secondary item (if any). Otherwise it's infinite.
  int32_t second = structure.secondaryItemsCount <= 0 ? maxInt : structure.ITEM.at(structure.primaryItemsCount);

  int32_t level = 0;
  int32_t smallestItemSizeAvailable = 0;

  // "Global" variables
  int32_t previousActive = active; // Value of "active" just before items of the current option get deactivated

  // Trail is a sequential stack, stores data for backtracking faster, without recomputing previous values
  auto TRAIL = std::vector<std::pair<int32_t, int32_t>>();

  // ALGORITHM C (Exact Covering with colors).

  int32_t bestItemIndex = 0;
  int32_t currentItemIndexChosen = 0;
  int32_t currentNodeIndex = 0; //
  constexpr int32_t invalidChoice = -1;
  // The node chosen on each level
  std::vector<int32_t> choices(structure.optionsCount, invalidChoice);
  // The size of savestack on each level
  std::vector<int32_t> saved(structure.optionsCount + 1, 0);
  auto saveStack = std::vector<std::pair<int32_t, int32_t>>();
  int32_t currentSaveIndex = 0;

  level = 0;
Forward: {
  smallestItemSizeAvailable = maxInt;
  // set best_itm to the best item for branching
  pickItemUsingMrvHeuristic(structure, randomGenerator, smallestItemSizeAvailable, bestItemIndex, active, second);

  const bool notAbleToPickItem = smallestItemSizeAvailable == maxInt;
  if (notAbleToPickItem) {
    // No item new item could be picked, therefore a solution was just found! Save it into the solutions vector.
    // Only the first elements in the choices list are the those that need saving.
    // The amount to save is equal to the current level, because a choice is made at every level.
    solutions.emplace_back(choices.begin(), choices.begin() + level);

    // Exit early if checking for uniqueness and more than one solution is found
    const bool uniquenessEarlyExit = checkForUniqueness && solutions.size() >= 2;
    // Exit early if it's not required to find all solutions and at least one has been found
    const bool singleSolutionEarlyExit = findFirstSolution && solutions.size() >= 1;
    if (singleSolutionEarlyExit || uniquenessEarlyExit) {
      goto Done;
    }

    goto Backup;
  }

  {
    // Swap the best item witht he last of the active list, making it inactive
    int32_t currentItemIndex = active - 1;
    active = currentItemIndex;
    int32_t indexOfBestItem = structure.position(bestItemIndex);
    int32_t setIndexOfCurrentItem = structure.ITEM[currentItemIndex];
    structure.ITEM[currentItemIndex] = bestItemIndex;
    structure.ITEM[indexOfBestItem] = setIndexOfCurrentItem;
    structure.position(setIndexOfCurrentItem) = indexOfBestItem;
    structure.position(bestItemIndex) = currentItemIndex;
  }

  {
    previousActive = active;
    hide(structure, bestItemIndex, 0, false, previousActive, second, active);
    currentItemIndexChosen = bestItemIndex;
  }

  // Save the currently active sizes
  {
    if (static_cast<size_t>(currentSaveIndex + active + 1) > saveStack.size()) {
      saveStack.resize(currentSaveIndex + active + 1);
    }
    for (int32_t p = 0; p < active; p++) {
      saveStack[currentSaveIndex + p] = {structure.ITEM[p], structure.size(structure.ITEM[p])};
    }
    currentSaveIndex = currentSaveIndex + active;
    saved[level + 1] = currentSaveIndex;
  }
}
Advance: {
  choices[level] = structure.SET[currentItemIndexChosen];
  currentNodeIndex = choices[level];
}
  // TryIt:
  {
    {
      // Swap out all other items of currentNodeIndex
      int32_t itemIndex = active;
      previousActive = active;
      for (int32_t siblingNodeIndex = currentNodeIndex + 1; siblingNodeIndex != currentNodeIndex;) {
        int32_t siblingNodeItem = structure.NODE[siblingNodeIndex].item;
        if (siblingNodeItem < 0) {
          // siblingNodeItem is a spacer, jump to the previous item in the option
          siblingNodeIndex += siblingNodeItem;
        } else {
          int32_t setIndexOfSiblingNodeItem = structure.position(siblingNodeItem);
          if (setIndexOfSiblingNodeItem < itemIndex) {
            // Swap out the item
            int32_t previousItemIndex = structure.ITEM[--itemIndex];
            structure.ITEM[itemIndex] = siblingNodeItem;
            structure.ITEM[setIndexOfSiblingNodeItem] = previousItemIndex;
            structure.position(previousItemIndex) = setIndexOfSiblingNodeItem;
            structure.position(siblingNodeItem) = itemIndex;
          }
          siblingNodeIndex++;
        }
      }
      active = itemIndex;
    }

    {
      // Hide the other options of those items or goto abort
      // A secondary item was purified at lower levels if and only if its position is >= previousActive
      for (int32_t siblingNodeIndex = currentNodeIndex + 1; siblingNodeIndex != currentNodeIndex;) {
        int32_t siblingNodeItem = structure.NODE[siblingNodeIndex].item;
        if (siblingNodeItem < 0) {
          // siblingNodeItem is a spacer, jump to the previous item in the option
          siblingNodeIndex += siblingNodeItem;
        } else {
          if (siblingNodeItem < second) {
            const bool isHideSuccessful = hide(structure, siblingNodeItem, 0, true, previousActive, second, active);
            if (!isHideSuccessful) {
              goto Abort;
            }
          } else { // do nothing if cc already purified
            int32_t pp = structure.position(siblingNodeItem);
            if (pp < previousActive) {
              const bool isHideSuccessful = hide(structure,
                                                 siblingNodeItem,
                                                 structure.NODE[siblingNodeIndex].color,
                                                 true,
                                                 previousActive,
                                                 second,
                                                 active);
              if (!isHideSuccessful) {
                goto Abort;
              }
            }
          }
          siblingNodeIndex++;
        }
      }
    }
    level++;
    goto Forward;
  }

Backup: {
  // When the level reaches 0, all possible solutions have been evaluated
  const bool searchedThroughAllPossibilities = level == 0;
  if (searchedThroughAllPossibilities) {
    goto Done;
  }
  level--;
  currentNodeIndex = choices[level];
  bestItemIndex = structure.NODE[currentNodeIndex].item;
  currentItemIndexChosen = structure.NODE[currentNodeIndex].location;
}
Abort: {

  if (currentItemIndexChosen + 1 >= bestItemIndex + structure.size(bestItemIndex)) {
    // Tried every option of the current best item
    goto Backup;
  }

  {
    // Restore the currently active sizes:
    // Use savestack to restore the size of the current best item in the structure
    currentSaveIndex = saved[level + 1];
    active = currentSaveIndex - saved[level];
    for (int32_t negativeItemIndex = -active; negativeItemIndex < 0; negativeItemIndex++) {
      structure.size(saveStack[currentSaveIndex + negativeItemIndex].first) =
          saveStack[currentSaveIndex + negativeItemIndex].second;
    }
  }
  // There's still options available for the current best item: go to the next
  currentItemIndexChosen++;
  goto Advance;
}
Done: { return retrieveOptionIndices(structure, solutions); }
}

} // namespace

std::vector<std::unordered_set<int32_t>> AlgorithmC::findAllSolutions(DancingCellsStructure& dataStructure,
                                                                      const std::optional<int32_t>& seed) {
  // Algorithm C returns a list of node indices. These are the first nodes of the options which describe the solution
  return runAlgorithmC(dataStructure, false, false, seed);
}

std::optional<std::unordered_set<int32_t>> AlgorithmC::findOneSolution(DancingCellsStructure& dataStructure,
                                                                       const std::optional<int32_t>& seed) {
  // Algorithm C returns a list of node indices. These are the first nodes of the options which describe the solution
  const auto solutions = runAlgorithmC(dataStructure, true, false, seed);
  if (solutions.size() >= 1) {
    return *solutions.begin();
  }
  return {};
}

std::optional<std::unordered_set<int32_t>> AlgorithmC::hasUniqueSolution(DancingCellsStructure& dataStructure,
                                                                         const std::optional<int32_t>& seed) {
  const auto solutions = runAlgorithmC(dataStructure, false, true, seed);
  if (solutions.empty()) {
    return {};
  }
  if (solutions.size() > 1) {
    return {};
  }
  // There's only one solution
  return *solutions.begin();
}
