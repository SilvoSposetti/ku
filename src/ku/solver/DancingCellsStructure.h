#pragma once
#include "DancingCellsNode.h"
#include "XccOption.h"

#include <unordered_map>
#include <vector>

/** Data structure used by Algorithm C.
 * This data structure mainly contains three lists: ITEM, SET and NODE.
 * Suppose ITEM[k]=i. Then the SET array, beginning at SET[i] lists the places where item i appears in options; and so,
 * if SET[i] = j, NODE[j].LOC = i.
 * The NODE array lists the options themselves.
 *
 * Whenever Algorithm C runs, it maintains the following invariants:
 * pos(ITEM[k]) = k -> SET[ITEM[k] - 2] = k
 * NODE[SET[q]].LOC = q
 * ITEM[pos(x)] = x -> ITEM[SET[x - 2]] = x
 * SET[NODE[y].LOC] = y
 */
class DancingCellsStructure {
public:
  /** Constructor
   * @param primaryItemsCount The amount of primary items (for n primary items: IDs: [0, 1, ..., n-1])
   * @param secondaryItemsCount The amount of secondary items (for m secondary items: IDs: [n, n+1, ..., n+m-1])
   * @param options The list of options, each option must contain sorted IDs, therefore the primary items appear first
   * in a single option's list, the secondary items appear at the end.
   */
  DancingCellsStructure(int32_t primaryItemsCount, int32_t secondaryItemsCount, const std::vector<XccOption>& options);

  /** Utility to log the current data structure contents to standard output. Useful for debugging.
   */
  void print() const;

  /** This function is a helper to retrieve the ITEM index of an option. It is simply an abbrevbiation for SET[x−2].
   * @param itemIndex The index of the item in the SET array.
   * @return A reference to the where the item is in the ITEM array.
   */
  int32_t& pos(int32_t itemIndex);

  /** This function is a helper to retrieve the amount of nodes (size) of an item. It is simply an abbrevbiation for
   * SET[x−1].
   * @param itemIndex The index of the item in the SET array.
   * @return A reference to how many active options there are for the particular item in the SET array.
   */
  int32_t& size(int32_t index);

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
  const int32_t primaryItemsCount = 0;
  /// The amount of secondary items
  const int32_t secondaryItemsCount = 0;
  /// The total amount of items
  const int32_t itemsCount = 0;
  /// The total amount of options
  const int32_t optionsCount = 0;

  /** This map is used to retrieve the option indices when Algorithm C has found a solution.
   * It must contain all node indices stored the NODE array as keys, and maps them to the option index that they
   * originally came from.
   */
  std::unordered_map<int32_t, int32_t> nodeIndicesToOptionIdMap;
};