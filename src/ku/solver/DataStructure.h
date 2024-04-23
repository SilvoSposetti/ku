#pragma once
#include "../Sudo.h"
#include "../constraints/AbstractConstraint.h"
#include "ItemData.h"
#include "Node.h"
#include "OptionData.h"

#include <vector>

/** Data structure used by Algorithm X.
 *It is essentially two-dimensional doubly-linked-list whose elements sit into a single, linearized vector of nodes.
 * Nodes contain "pointers" to other elements in the structure in the form of integer indices inside the vector.
 * The structure is composed of multiple sections packed into a single vector of Nodes:
 * 1. A root node.
 * 2. A list of header nodes. A header is given to every item.
 *    - Header nodes make a circular doubly linked-list together with the root node.
 *    - Header nodes have a length attribute that keeps track of how many elements are contained within the item.
 * 3. A list of all the non-zero matrix entries in row-major order. Different rows are separated by
 *   spacer nodes. These spacer nodes appear at both ends of these entries as well.
 *   And additionally:
 *    - Every node of the sparse coordinate matrix composes a 2-dimensional doubly-linked list because it holds:
 *       - "pointers" to its left & right neighbor nodes, these are implicitly the current node's index + 1 and - 1.
 *       - "pointers" to uts up & down neighbors nodes, these are stored in the node itself.
 *    - Every node also holds a "pointer" to its header.
 * In particular, a spacer node N's up and down links are:
 *    - Up-link: points to the first node in the option before N.
 *    - Down-link: points to the last node in the option after N.
 */
class DataStructure {
public:
  /** Constructor. Creates the structure given a board and a set of constraints.
   * @param board The board
   * @param constraints The set of constraints
   */
  DataStructure(const std::vector<std::vector<Sudo::Digit>>& board,
                const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  /** Constructor. Creates the structure given a traditional binary matrix.
   * @param matrix The binary matrix
   * @param secondaryItemsAmount How many items are secondary counting from the right-end of the matrix
   */
  DataStructure(const std::vector<std::vector<bool>>& matrix, int32_t secondaryItemsAmount = 0);

  /* A copy of the internal structure, to use for a run of Algorithm X.
   * @return The structure, ready to be used by Algorithm X
   */
  std::vector<Node> getStructureCopy() const;

  /* Retrieves the amount of items available in the structure
   * @return The amount of items
   */
  int32_t getItemsAmount() const;

  /* Retrieves data about all the items in the structure
   * @return A list of items' data
   */
  const std::vector<ItemData>& getItemsData() const;

  /* Retrieves the amount of options available in the structure
   * @return The amount of options
   */
  int32_t getOptionsAmount() const;

  /* Retrieves data about all the options in the structure
   * @return A list of options' data
   */
  const std::vector<OptionData>& getOptionsData() const;

  /* Computes whether a structure cannot be solved by Algorithm X purely based on its features.
   * Note that a structure marked as "solvable" by this function might still not have any solutions.
   * The structure is not solvable if all items are secondary, or if a primary item is empty.
   * @return Whether the structure may be solvable by AlgorithmX
   */
  bool isPotentiallySolvableByAlgorithmX() const;

  /* Helper to log the structure to standard output
   */
  void print() const;

private:
  /* Common helper function that constructs the structure based on the input provided
   * @param items The items' data
   * @param options The options' data
   * @param optionsCache A list of all the non-zero elements' indices present in each option.
   * @param nonZeroElementsAmount The total sum of all the non-zero elements
   * @return The constructed structure
   */
  static std::vector<Node> createStructure(const std::vector<ItemData>& items,
                                           const std::vector<OptionData>& options,
                                           const std::vector<std::vector<int32_t>>& optionsCache,
                                           int32_t nonZeroElementsAmount);

private:
  /* Data about the items
   */
  std::vector<ItemData> itemsData;

  /* The data about the options
   */
  std::vector<OptionData> optionsData;

  /* The structure, directly modified when Algorithm X runs
   */
  std::vector<Node> structure;

  bool isSolvable = false;
};