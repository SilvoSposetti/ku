#pragma once
#include "DancingCellsNode.hpp"
#include "OptionData.hpp"

#include <vector>

/** Data structure used by Algorithm C.
 * This data structure mainly contains three lists: ITEM, SET and NODE. These three lists are modified by Algorithm C as
 * it runs.
 *
 * Whenever Algorithm C runs, it maintains the following invariants:
 * pos(ITEM[k]) = k -> SET[ITEM[k] - 2] = k
 * NODE[SET[q]].LOC = q
 * ITEM[pos(x)] = x -> ITEM[SET[x - 2]] = x
 * SET[NODE[y].LOC] = y
 */
struct DancingCellsStructure {
public:
  /** Constructor
   * @param primaryItemsCount The amount of primary items (for n primary items: IDs: [0, 1, ..., n-1])
   * @param secondaryItemsCount The amount of secondary items (for m secondary items: IDs: [n, n+1, ..., n+m-1])
   * @param options The list of options, each option must contain sorted IDs, therefore the primary items appear first
   * in a single option's list, the secondary items appear at the end.
   */
  DancingCellsStructure(int32_t primaryItemsCount,
                        int32_t secondaryItemsCount,
                        const std::vector<std::vector<XccElement>>& options);

  /** This function is a helper to retrieve the ITEM index of an option.
   * It is simply an abbrevbiation for SET[x−2] for an input x.
   * @param itemIndex The index of the item in the SET array.
   * @return A reference to the where the item is in the ITEM array.
   */
  inline int32_t& position(int32_t itemIndex) {
    return SET[itemIndex - 2];
  };

  /** This function is a helper to retrieve the amount of nodes (aka size) of an item.
   * It is simply an abbrevbiation for SET[x−1] for an input x.
   * @param itemIndex The index of the item in the SET array.
   * @return A reference to how many active options there are for the particular item in the SET array.
   */
  inline int32_t& size(int32_t itemIndex) {
    return SET[itemIndex - 1];
  };

private:
  /** Helper to make sure that a set of preconditions are ensured before creating the structure with some options.
   * @param options The list of options
   * @param primaryItemsCount The amount of primary items
   * @param itemsCount The toal amount of items (primary + secondary).
   */
  static void
  checkOptions(const std::vector<std::vector<XccElement>>& options, int32_t primaryItemsCount, int32_t itemsCount);

  /** Helper to allocate the correct amount of memory necessary for the ITEM, SET, and NODE lists.
   * @param primaryCount The amount of primary items.
   * @param secondaryCount The amount of secondary items.
   * @param optionsCount The amount of options.
   * @param nodesCount The amount of nodes.
   */
  void allocateMemoryForMembers(int32_t primaryCount, int32_t secondaryCount, int32_t optionsCount, int32_t nodesCount);

  /** Helper to set up the temporary data in NODE and SET. Used whilst going through the options once at the beginning
   * of the structure creation.
   * @param element The element's data
   * @param lastNode The index of the last node, is modified by this function
   * @param optionIndex The index of the option in which this node was in
   */
  void createNodeForItem(const XccElement& element, int32_t& lastNode, int32_t optionIndex);

  /** Helper to finish initialization once NODE and SET have been set up temporarily by visiting all options once.
   * @param second If secondary items were encountered, it contains the value for the boundary between primary and
   * secondary items
   * @param lastNode The index of the last node
   */
  void finishInitialization(int32_t second, int32_t lastNode);

public:
  // /** Utility to log the current data structure contents to standard output. Useful for debugging.
  //  */
  // void print() const;

public:
  /** ITEM contains all items in the domain. The value of ITEM[k] is an index x into SET, for which SET[x-1] (size) and
   * SET[x-2] (pos) are valid values.
   * The set of all options that involve the k-th item appears in the SET array at index k.
   */
  std::vector<int32_t> ITEM;

  /** SET is an heterogeneous list containing blocks of data. Each block is made up of three different data-pieces.
   * The first element of a block contains the index of the block's item in the ITEM list.
   * The second element of a block contains the amount of (active) nodes that the item currently has. (i.e. the size of
   * the following list).
   * The third element of a block is a list of nodes of the item. Each location in this list stores an index which
   * points to a node in the NODE list.
   *  [
   *    // first block, which relates to the first item
   *    pos(A) in ITEM.
   *    size(A) in ITEM.
   * A: - first Node of first item
   *    - second Node of first item
   *    - third Node of first item
   *    - ...
   *    // second block, which relates to the second item
   *    pos(B) in ITEM.
   *    size(B) in ITEM.
   * B: - first Node of second item
   *    - second Node of second item
   *    - third Node of second item
   *    - ...
   *    // third block, which relates to the third item, and so on...
   * ]
   * In the list, A,B, and so on are the main "reference" nodes that are used when retrieving valid pos() or size() data
   * of an item.
   */
  std::vector<int32_t> SET;

  /** NODE is also an heterogeneous array. It contains blocks of data that relate to each given options one by one.
   * Each option is separated by spacer, with a spacer at the beginning and a spacer at the end of the array.
   * For an option, the block of data stored is a DancingCellsNode list for each node in the option.
   * [
   *    // Spacer
   * A: - first Node of first option
   *    - second Node of first option
   *    - third Node of first option
   *    - ...
   *    // Spacer
   * B: - first Node of second option
   *    - second Node of second option
   *    - third Node of second option
   *    - ...
   *    // Spacer
   * ]
   * Spacer nodes are special DancingCellsNodes. For an option with l nodes:
   * - the spacer before that option has location value equal to l.
   * - the spacer after that option has item value equal to -l.
   */
  std::vector<DancingCellsNode> NODE;

  /// The amount of primary items
  int32_t primaryItemsCount = 0;
  /// The amount of secondary items
  int32_t secondaryItemsCount = 0;
  /// The total amount of items
  int32_t itemsCount = 0;
  /// The total amount of options
  int32_t optionsCount = 0;

  /** This map is used to retrieve the option indices when Algorithm C has found a solution.
   * It contains an option ID for every node.
   */
  std::vector<int32_t> nodeOptionIndices;

  /** List of data for every option. When the structure is created to represent a puzzle, it contains information to
   * find out which cell and possibility an option was created to represent.
   */
  std::vector<OptionData> optionsData;
};
