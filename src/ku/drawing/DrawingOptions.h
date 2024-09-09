#pragma once

#include <cstdint>

/** Options for drawing puzzles
 */
struct DrawingOptions {

  /** Constructor
   * @param size The size of the puzzle
   * @param margin The margin around the puzzle
   * @param constraintsCount The amount of constraints in the puzzle
   */
  DrawingOptions(double size, double margin, int32_t constraintsCount);

  // Size of the puzzle
  const double size = 0;
  // Margin of the puzzle
  const double margin = 0;
  // Amount of constraints
  const int32_t constraintsCount = 0;

  // Size of a single cell
  const double cellSize = 0;

  // Font size for given digits
  const double givenDigitsFontSize = 0;
  // Font size for non-given digits
  const double nonGivenDigitsFontSize = 0;
  // Font size for the title
  const double titleFontSize = 0;
  // Base height for the title
  const double titleBaseHeight = 0;
  // Font size for additional information
  const double infoFontSize = 0;
  // Line height for additional information
  const double infoLineHeight = 0;
  // Total height of the additional information
  const double infoHeight = 0;
  // Base height for the additional information
  const double infoBaseHeight = 0;

  // Totalheight of the puzzle
  const double totalHeight = 0;

  // Thin line lenght
  const double thinLine = 0;
  // Medium line lenght
  const double mediumLine = 0;
  // Thick line lenght
  const double thickLine = 0;
};
