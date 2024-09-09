#pragma once

#include <memory>
#include <string>
#include <vector>

/** Base class for any SVG element
 */
class SvgElement {
public:
  /** Constructor
   * @param tagName The name of the tag
   * @param canHaveChildElements Whether child elements are allowed
   */
  SvgElement(const std::string& tagName, bool canHaveChildElements);

  /** Destructor
   */
  virtual ~SvgElement() = default;

  /** Computes the current element's string representation
   * @return The string representation of a valid SVG element
   */
  virtual std::string string() const = 0;

  /** Adds another entity as a child of this one. Will only add the entity if allowed, otherwise does nothing.
   * @param entity The other entity
   */
  void add(std::unique_ptr<SvgElement> entity);

  /** Utility to transform numbers to string, rounding them to some digits after the comma
   * @param number A number
   * @return The rounded number as a string
   */
  static std::string number(double number);

  /** Prefixes a string with the provided characters such that the returend one is at least n characters long
   * @param input The input string
   * @param character the character to be used for padding
   * @param n The total amount of characters required
   * @return The input padded on the left
   */
  static std::string padLeft(const std::string& input, char character, int32_t n);

protected:
  /** The name of the tag
   */
  const std::string tagName;

  /** Whether this element allows to store child elements within it
   */
  const bool canHaveChildElements;

  /** A list of all the child elements
   */
  std::vector<std::unique_ptr<SvgElement>> childElements;
};