#pragma once
#include <string>

/** Holds information about an item
 */
struct ItemData {
  /** Default constructor
   */
  ItemData() = default;

  /** Constructor
   * @param constraintName The name of the constraint that defines the item
   * @param isPrimary Whether the item is primary
   * @param itemID The item id for the constraint
   */
  ItemData(const std::string& constraintName, bool isPrimary, int32_t itemId);

  /// The name of the constraint that defines the item
  std::string constraintName;
  /// Whether the item is primary
  bool isPrimary = true;
  /// The item id for the constraint
  int32_t itemId = -1;
};
