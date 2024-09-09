#pragma once

#include "SvgElement.h"

#include <filesystem>

/** SVG Document
 */
class SvgDocument : public SvgElement {
public:
  /** Constructor
   * @param mame The name of the document
   * @param width The width. The document will then span from [0, width] in the X-direction
   * @param width The height. The document will then span from [0, height] in the Y-direction
   * @param margin A margin to be applied in all directions
   */
  SvgDocument(const std::string& name, double width, double height, double margin);

  virtual std::string string() const override;

  /** Adds a background that covers the whole viewport of the document
   * @param fill The fill for the background
   */
  void addBackground(const std::string& fill);

  /** Writes the document's string to a file specified. Filename will be the {name}.svg
   * @param path The directory into which to write the document.
   */
  void writeToFile(const std::filesystem::path& path) const;

private:
  const std::string name;
  const double width = 0;
  const double height = 0;
  const double margin = 0;
};