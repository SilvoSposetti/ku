#include "AlgorithmC.h"

#include "../randomGenerator/RandomGenerator.h"

#include <algorithm>
#include <functional>
#include <limits>

/** Helper functions for Algorithm C have been put here in an unnamed namespace such that they are only accessible by
 * this translation unit and nowhere else
 */
namespace {

/** Selects one of the of the item with smallest length in the active list of the data structure. If there are
 * multiple elements with the same smallest length, selects one of those at random.
 * @param structure A reference to the structure currently being used
 * @return The index to the first smallest item in the data structure
 */
void pickUsingMrvHeuristic(DancingCellsStructure& structure,
                           RandomGenerator& randomGenerator,
                           int32_t& t,
                           int32_t& best_itm,
                           int32_t ACTIVE,
                           int32_t SECOND) {
  int32_t minimumLengthNodesAmount = 0;
  const auto selectThisItem = [&](int32_t k, int32_t s) {
    best_itm = structure.ITEM[k];
    t = s;
  };

  for (int32_t k = 0; t > 1 && k < ACTIVE; k++) {
    if (structure.ITEM[k] < SECOND) {
      int32_t s = structure.size(structure.ITEM[k]);
      if (s < t) {
        selectThisItem(k, s);
        minimumLengthNodesAmount = 1;
      } else if (s == t) {
        minimumLengthNodesAmount++;
        if (randomGenerator.uniformFloat(0.0f, 1.0f) < (1.0f / static_cast<float>(minimumLengthNodesAmount))) {
          selectThisItem(k, s);
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
      solution.insert(structure.nodeIndicesToOptionIdMap.at(value));
    }
    solutions.push_back(solution);
  }
  return solutions;
};

int32_t hide2(DancingCellsStructure& structure,
              int32_t c,
              int32_t color,
              int32_t check,
              int32_t OACTIVE,
              int32_t SECOND,
              int32_t ACTIVE) {
  int32_t rr = c;
  int32_t s = c + structure.size(c);

  for (; rr < s; rr++) {
    int32_t tt = structure.SET[rr];
    if (!color || structure.NODE[tt].color != color) {
      //  remove option tt from the other sets it's in
      {
        for (int32_t nn = tt + 1; nn != tt;) {
          int32_t uu = structure.NODE[nn].item;
          int32_t vv = structure.NODE[nn].location;
          if (uu < 0) {
            nn += uu;
            continue;
          }
          if (structure.pos(uu) < OACTIVE) {
            int32_t ss = structure.size(uu) - 1;
            if (ss == 0 && check && uu < SECOND && structure.pos(uu) < ACTIVE) {
              return 0;
            }
            int32_t nnp = structure.SET[uu + ss];
            structure.size(uu) = ss;
            structure.SET[uu + ss] = nn;
            structure.SET[vv] = nnp;
            structure.NODE[nn].location = uu + ss;
            structure.NODE[nnp].location = vv;
          }
          nn++;
        }
      }
    }
  }
  return 1;
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
std::vector<std::unordered_set<int32_t>> runAlgorithmC(DancingCellsStructure& structure,
                                                       bool findFirstSolution,
                                                       bool checkForUniqueness,
                                                       const std::optional<int32_t>& seed) {

  const int32_t optionsAmount = structure.optionsCount;
  // Don't run Algorithm C on an empty structure
  if (optionsAmount == 0) {
    return {};
  }

  // // Get a copy of the structure to work on
  // auto structure = dataStructure;

  // apparentlyTheresAnotherX is the list of all options currently selected, this is used for backtracking
  // an option in this case is represented by the index of a node that is contained by the option
  // std::vector<int32_t> apparentlyTheresAnotherX(optionsAmount, -1);

  // The following vector will store the solutions, i.e. the sets of nodes which are contained by the options that
  // solve the exact cover problem
  std::vector<std::unordered_set<int32_t>> solutions;

  // Initialize random generator to pick columns randomly according to a certain seed
  RandomGenerator randomGenerator(seed);

  int32_t ACTIVE = structure.itemsCount;
  // Second is the internal number of the smallest secondary item (if any). Otherwise it's infinite.
  int32_t SECOND = std::numeric_limits<int32_t>::max();
  if (structure.secondaryItemsCount > 0) {
    SECOND = structure.ITEM.at(structure.primaryItemsCount);
  }

  int32_t l = 0;
  int32_t t = 0;

  // "Global" variables
  // int32_t FLAG = 0; // Set when need to backtrack immediately
  int32_t OACTIVE = ACTIVE; // Value of ACTIVE just before items of the current option are being deactivated

  // Trail is a sequential stack, stores data for backtracking faster, without recomputing
  auto TRAIL = std::vector<std::pair<int32_t, int32_t>>();

  // The index of the item in structure.SET that's been chosen
  // int32_t i = 0;
  // The index of the option containing the item in structure.SET
  // int32_t j = 0;

  // ALGORITHM C (Exact Covering with colors).

  int32_t max_nodes = std::numeric_limits<int32_t>::max();
  int32_t best_itm = 0; // i of before
  int32_t cur_choice = 0; // j of before
  int32_t cur_node = 0; //
  int32_t invalidChoice = -1;
  std::vector<int32_t> choice(optionsAmount, invalidChoice); // The node chosen on each level
  std::vector<int32_t> saved(optionsAmount + 1, 0); // The size of savestack on each level
  auto savestack = std::vector<std::pair<int32_t, int32_t>>();
  int32_t saveptr = 0;

  l = 0;
forward: {
  t = max_nodes;
  // set best_itm to the best item for branching
  pickUsingMrvHeuristic(structure, randomGenerator, t, best_itm, ACTIVE, SECOND);

  if (t == max_nodes) {
    // Solution found! Save it into the solutions vector
    // Only the first choices are the those that need saving. The amount is equal to the current level
    auto solution = std::unordered_set<int32_t>(choice.begin(), choice.begin() + l);
    solution.erase(invalidChoice);
    solutions.push_back(solution);

    // Exit early if checking for uniqueness and more than one solution is found
    const bool uniquenessEarlyExit = checkForUniqueness && solutions.size() >= 2;
    // Exit early if it's not required to find all solutions and at least one has been found
    const bool singleSolutionEarlyExit = findFirstSolution && solutions.size() >= 1;
    if (singleSolutionEarlyExit || uniquenessEarlyExit) {
      goto done;
    }

    goto backup;
  }

  // swap best_itm out of the active list
  {
    int32_t p = ACTIVE - 1;
    ACTIVE = p;
    int32_t pp = structure.pos(best_itm);
    int32_t cc = structure.ITEM[p];
    structure.ITEM[p] = best_itm;
    structure.ITEM[pp] = cc;
    structure.pos(cc) = pp;
    structure.pos(best_itm) = p;
  }

  {
    OACTIVE = ACTIVE;
    hide2(structure, best_itm, 0, 0, OACTIVE, SECOND, ACTIVE);
    cur_choice = best_itm;
  }

  // Save the currently active sizes
  {
    if (saveptr + ACTIVE + 1 > savestack.size()) {
      savestack.resize(saveptr + ACTIVE + 1);
    }

    for (int32_t p = 0; p < ACTIVE; p++) {
      savestack[saveptr + p] = {structure.ITEM[p], structure.size(structure.ITEM[p])};
    }
    saveptr = saveptr + ACTIVE;
    saved[l + 1] = saveptr;
  }
}
advance: {
  choice[l] = structure.SET[cur_choice];
  cur_node = choice[l];
}
tryit: {
  // Swap out all other items of cur_node
  {
    int32_t p = ACTIVE;
    OACTIVE = ACTIVE;
    for (int32_t q = cur_node + 1; q != cur_node;) {
      int32_t c = structure.NODE[q].item;
      if (c < 0) {
        q += c;
      } else {
        int32_t pp = structure.pos(c);
        if (pp < p) {
          int32_t cc = structure.ITEM[--p];
          structure.ITEM[p] = c;
          structure.ITEM[pp] = cc;
          structure.pos(cc) = pp;
          structure.pos(c) = p;
        }
        q++;
      }
    }
    ACTIVE = p;
  }

  // Hide the other options of those items or goto abort
  {
    // A secondary item was purified at lower levels if and only if its position is >= OACTIVE
    for (int32_t q = cur_node + 1; q != cur_node;) {
      int32_t cc = structure.NODE[q].item;
      if (cc < 0) {
        q += cc;
      } else {
        if (cc < SECOND) {
          if (0 == hide2(structure, cc, 0, 1, OACTIVE, SECOND, ACTIVE)) {
            goto abort;
          }
        } else { // do nothing if cc already purified
          int32_t pp = structure.pos(cc);
          if (pp < OACTIVE && 0 == hide2(structure, cc, structure.NODE[q].color, 1, OACTIVE, SECOND, ACTIVE)) {
            goto abort;
          }
        }
        q++;
      }
    }
  }
  l++;
  goto forward;
}

backup: {
  const bool searchedThroughAllPossibilities = l == 0;
  if (searchedThroughAllPossibilities) {
    // When the level reaches 0, all possible solutions have been evaluated
    goto done;
  }
  l--;
  cur_node = choice[l];
  best_itm = structure.NODE[cur_node].item;
  cur_choice = structure.NODE[cur_node].location;
}
abort: {

  if (cur_choice + 1 >= best_itm + structure.size(best_itm)) {
    // Tried every option of the current best item
    goto backup;
  }

  // Restore the currently active sizes
  {
    // Use savestack to restore the size of the current best item in the structure
    saveptr = saved[l + 1];
    ACTIVE = saveptr - saved[l];
    for (int32_t p = -ACTIVE; p < 0; p++) {
      structure.size(savestack[saveptr + p].first) = savestack[saveptr + p].second;
    }
  }
  // There's still options available for the current best item, go to the next
  cur_choice++;
  goto advance;
}
done: { return retrieveOptionIndices(structure, solutions); }
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
