#include "DrawingOptions.hpp"

DrawingOptions::DrawingOptions(double size, double margin, int32_t constraintsCount)
    : size(size)
    , margin(margin)
    , constraintsCount(constraintsCount)
    , cellSize(size / 9.0)
    , givenDigitsFontSize(cellSize * 0.618)
    , nonGivenDigitsFontSize(givenDigitsFontSize / 5.0)
    , titleFontSize(givenDigitsFontSize / 3)
    , titleBaseHeight(size + margin + titleFontSize)
    , infoFontSize(titleFontSize / 1.5)
    , infoLineHeight(infoFontSize * 1.5)
    , infoHeight(infoLineHeight * (constraintsCount - 1))
    , infoBaseHeight(size + margin + titleFontSize * 1.5 + infoLineHeight)
    , totalHeight(infoBaseHeight + infoHeight)
    , thinLine(size / 800.0)
    , mediumLine(thinLine * 3)
    , thickLine(mediumLine * 2) {}
