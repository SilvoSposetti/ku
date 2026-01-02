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
