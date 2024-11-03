#pragma once
#include <cstdint>

/** An elment of an XCC option represents either a primary or secondary item.
 * Secondary items are allowed to have colors.
 */
struct XccElement {

  /** Static construction method to create a primary item with a given ID.
   * @param id The ID.
   * @return A constructed primary XccElement.
   */
  static XccElement makePrimary(int32_t id);

  /** Static construction method to create a secondary item with a given ID and non-specified color.
   * @param id The ID.
   * @return A constructed secondary XccElement that does not specify a color.
   */
  static XccElement makeSecondary(int32_t id);

  /** Static construction method to create a secondary item with a given ID and specific color.
   * @param id The ID of the item.
   * @param colorId The ID of the color.
   * @return A constructed secondary XccElement with the specific color.
   */
  static XccElement makeSecondary(int32_t id, int32_t colorId);

  /// The id of the element
  const int32_t id = -1;
  /// Wheter the element is primary
  const bool isPrimary = true;
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

private:
  /** Private constructor such that only static construction methods are used.
   * @param id The ID of the item.
   * @param isPrimary Whether the element is primary.
   * @param colorId The ID of the color.
   */
  XccElement(int32_t id, bool isPrimary, int32_t colorId);
};
