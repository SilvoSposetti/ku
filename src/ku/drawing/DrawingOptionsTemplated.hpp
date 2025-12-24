#pragma once

#include "../puzzles/PuzzleSpace.hpp"

#include <cstdint>

/** Options for drawing puzzles
 */
template <PuzzleSpace puzzleSpace>
struct DrawingOptionsTemplated {

  /** Constructor
   * @param size The core size of the puzzle
   * @param margin The margin around the puzzle
   * @param constraintsCount The amount of constraints in the puzzle
   */
  DrawingOptionsTemplated(double size, double margin, int32_t constraintsCount)
      : width(size)
      , margin(margin)
      , constraintsCount(constraintsCount)
      , cellSize(width / puzzleSpace.columnsCount)
      , height(cellSize * puzzleSpace.rowsCount)
      , givenDigitsFontSize(cellSize * 0.618)
      , nonGivenDigitsFontSize(givenDigitsFontSize / 5.0)
      , titleFontSize(width / 30)
      , titleBaseHeight(height + margin + titleFontSize)
      , infoFontSize(titleFontSize / 1.5)
      , infoLineHeight(infoFontSize * 1.5)
      , infoHeight(infoLineHeight * (constraintsCount - 1))
      , infoBaseHeight(height + margin + titleFontSize * 1.5 + infoLineHeight)
      , totalHeight(infoBaseHeight + infoHeight)
      , thinLine(titleFontSize / 25.0)
      , mediumLine(thinLine * 3)
      , thickLine(mediumLine * 2) {}

  // Width of the puzzle
  const double width = 0;
  // Margin of the puzzle
  const double margin = 0;
  // Amount of constraints
  const int32_t constraintsCount = 0;
  // Size of a single cell
  const double cellSize = 0;
  // Height of the puzzle
  const double height = 0;
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
  // Total height of the puzzle
  const double totalHeight = 0;
  // Thin line lenght
  const double thinLine = 0;
  // Medium line lenght
  const double mediumLine = 0;
  // Thick line lenght
  const double thickLine = 0;
};
