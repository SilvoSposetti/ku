#pragma once
#include "XccElement.hpp"

#include <compare>
#include <cstdint>

/** Struct of data stored in the NODE array of the DancingCellsStructure.
 */
struct DancingCellsNode {
  /** Default comparison operator
   */
  auto operator<=>(const DancingCellsNode& other) const = default;

  /** The index of the first (reference) node of an item in the SET array for this node.
   * Remains constant througout an Algorithm C's run.
   */
  int32_t item = 0;

  /** The index of the actual location of the current node in the SET array.
   * This is modified during Algorithm C's run.
   */
  int32_t location = 0;

  /** The color identification for the current option's node.
   * Remains constant througout an Algorithm C's run.
   */
  int32_t color = XccElement::undefinedColor();
};
