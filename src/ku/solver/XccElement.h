#pragma once
#include <cstdint>

/** An elment of an XCC option represents either a primary or secondary item.
 * Secondary items are allowed to have colors.
 */
struct XccElement {

  /** Static construction method to create a secondary item with a given ID and non-specified color.
   * @param id The ID.
   * @return A constructed secondary XccElement that does not specify a color.
   */
  XccElement(int32_t id);

  /** Static construction method to create a secondary item with a given ID and specific color.
   * @param id The ID of the item.
   * @param colorId The ID of the color.
   * @return A constructed secondary XccElement with the specific color.
   */
  XccElement(int32_t id, int32_t colorId);

  /// The id of the element
  const int32_t id = -1;
  /// The color of the element. For primary element this should be ignored.
  const int32_t colorId = undefinedColor();

  /** The color id representing an undefined color
   */
  static int32_t undefinedColor();

  /** operator< overload to be able to put XccElements in ordered sets.
   * This operator considers only the id member on purpose.
   * @param other The other instance of XccElement.
   * @return Whether this instance is less than the other.
   */
  const bool operator<(const XccElement& other) const {
    return (id < other.id);
  }
};
