#include "SvgUtilities.h"

#include <iomanip>

std::string SvgUtilities::getSvgHeader() {
  std::string header = "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"";

  // ViewBox
  header += toString(-boardMargin) + " " + toString(-boardMargin) + " " + toString(totalBoardSize + dlxMatrixWidth) +
            // " " + toString(totalBoardSize + infoHeight) + "\" font-family=\"monospace\" >\n";
            " " + toString(totalBoardSize + infoHeight) + "\" font-family=\"Open Sans\" >\n";

  return header;
}
std::string SvgUtilities::getSvgFooter() {
  return "</svg>\n";
  ;
}

std::string SvgUtilities::background() {
  return "<rect x=\"" + toString(-boardMargin) + "\" y=\"" + toString(-boardMargin) + "\" width=\"" +
         toString(totalBoardSize + dlxMatrixWidth) + "\" height=\"" + toString(totalBoardSize + infoHeight) + "\"" +
         lightRectStyle + "/>\n";
}

std::string SvgUtilities::createGroup(const std::string& name, const std::string& group, const std::string& style) {
  std::string groupString = "<g id=\"" + name + "\"";
  if (!style.empty()) {
    groupString += style;
  }
  groupString += ">\n";
  groupString += group;
  groupString += "</g>\n";
  return groupString;
}

std::string SvgUtilities::line(double x1, double y1, double x2, double y2, const std::string& style) {
  const double xPosition1 = x1 * boardSize;
  const double yPosition1 = y1 * boardSize;
  const double xPosition2 = x2 * boardSize;
  const double yPosition2 = y2 * boardSize;

  return "<line x1=\"" + toString(xPosition1) + "\" y1=\"" + toString(yPosition1) + "\" x2=\"" + toString(xPosition2) +
         "\" y2=\"" + toString(yPosition2) + "\"" + style + "/>\n";
}

std::string SvgUtilities::rect(double x, double y, double width, double height, const std::string& style) {
  const double xPosition = x * boardSize;
  const double yPosition = y * boardSize;
  const double rectWidth = width * boardSize;
  const double rectHeight = height * boardSize;

  return "<rect x=\"" + toString(xPosition) + "\" y=\"" + toString(yPosition) + "\" width=\"" + toString(rectWidth) +
         "\" height=\"" + toString(rectHeight) + "\"" + style + "/>\n";
}

std::string SvgUtilities::text(double x, double y, const std::string& text, const std::string& style) {
  return "<text x=\"" + toString(x) + "\" y=\"" + toString(y) + "\"" + style + ">" + text + "</text>\n";
}

std::string SvgUtilities::titleAndDescription(const std::string& sudokuName,
                                              const std::vector<std::string>& constraintDescriptions) {
  const int titleFontSize = boardSize / 40;
  const int descriptionFontSize = boardSize / 50;
  const int lineSeparation = boardSize / 35;

  const double baseY = boardSize + boardMargin + titleFontSize;

  // Title
  std::string title = text(0, baseY, sudokuName, getFontSize(titleFontSize));
  title = createGroup("Title", title, getFontSize(titleFontSize));

  // Description
  int32_t line = 0;
  std::string description;
  for (const auto& constraint : constraintDescriptions) {
    description += text(0, baseY + (line + 1) * lineSeparation, constraint);
    line++;
  }
  description = createGroup("Description", description, getFontSize(descriptionFontSize));
  return title + description;
}

std::string SvgUtilities::givenDigits(const std::vector<std::vector<Sudo>>& solution,
                                      const std::vector<std::vector<bool>>& givenMask) {
  const int givenDigitFontSize = boardSize / 13;
  const std::string givenDigitTextStyle =
      " text-anchor=\"middle\" dominant-baseline=\"central\" font-size=\"" + std::to_string(givenDigitFontSize) + "\"";

  std::string givenDigits;
  for (const auto& i : INDICES) {
    for (const auto& j : INDICES) {
      if (givenMask[i][j]) {
        const double cellCenterX = (j + 0.5) * cellSize;
        const double cellCenterY = (i + 0.5) * cellSize;
        const std::string digitString = std::to_string(static_cast<int32_t>(solution[i][j]));
        givenDigits += text(cellCenterX, cellCenterY, digitString);
      }
    }
  }
  givenDigits = createGroup("Given-Digits", givenDigits, givenDigitTextStyle);

  // Add also non-given digits
  const int nonGivenDigitFontSize = boardSize / 50;
  const std::string nonGivenDigitTextStyle = " text-anchor=\"middle\" dominant-baseline=\"central\" font-size=\"" +
                                             std::to_string(nonGivenDigitFontSize) + "\"";
  std::string nonGivenDigits;
  for (const auto& i : INDICES) {
    for (const auto& j : INDICES) {
      if (!givenMask[i][j]) {
        const double cellCenterX = (j + 0.5) * cellSize;
        const double cellCenterY = (i + 0.5) * cellSize;
        const std::string digitString = std::to_string(static_cast<int32_t>(solution[i][j]));
        nonGivenDigits += text(cellCenterX, cellCenterY, digitString);
      }
    }
  }
  nonGivenDigits = createGroup("Non-Given-Digits", nonGivenDigits, nonGivenDigitTextStyle);
  return givenDigits + nonGivenDigits;
}

std::string SvgUtilities::givenPattern(int32_t cellIndexI, int32_t cellIndexJ, bool isGiven) {
  const double x = boardSize + boardMargin + givenPatternCellSize * cellIndexJ;
  const double y = boardSize + boardMargin - givenPatternCellSize * ((MAX_DIGIT + 2) - cellIndexI);
  const std::string style = isGiven ? darkRectStyle : lightRectStyle;
  return paperUnitsRect(x, y, givenPatternCellSize, givenPatternCellSize, style);
}

std::string SvgUtilities::givenPatternBorder() {
  const double x = boardSize + boardMargin;
  const double y = boardSize + boardMargin - givenPatternCellSize * ((MAX_DIGIT + 2));
  const double size = givenPatternCellSize * MAX_DIGIT;
  return paperUnitsRect(x, y, size, size, getNoFillStroke(thinLine));
}

std::string SvgUtilities::dlxMatrix(const std::vector<std::vector<int32_t>>& matrix,
                                    const std::vector<std::pair<std::string, int32_t>>& constraintTexts,
                                    int32_t columnsAmount) {
  const double dlxCellSize = boardSize / (9. * 9. * 9.);
  const double originX = boardSize + boardMargin;
  const double originY = 0;
  const int32_t textSize = boardSize / 100;
  const int32_t textDistance = dlxCellSize * 5;
  const double constraintSeparation = dlxCellSize * 9;
  const int32_t rows = matrix.size();

  std::string result;
  // Keep track of which columns have already been considered
  int32_t columnsCounter = 0;
  int32_t constraintCounter = 0;
  for (const auto& constraint : constraintTexts) {
    const double constraintOriginX = originX + columnsCounter * dlxCellSize + constraintCounter * constraintSeparation;
    const std::string name = constraint.first;
    const int32_t constraintColumns = constraint.second;

    // Name
    double textPositionX = constraintOriginX + (constraintColumns * 0.5) * dlxCellSize;
    double textPositionY = originY - textDistance;
    std::string constraintText =
        text(textPositionX, textPositionY, name, getRotatedTextStyle(textPositionX, textPositionY, textSize));
    constraintText = createGroup("DLX-" + name + "-Name", constraintText);

    // Background
    std::string constraintBackground =
        paperUnitsRect(constraintOriginX, originY, dlxCellSize * constraintColumns, dlxCellSize * rows, whiteRectStyle);
    constraintBackground = createGroup("DLX-" + name + "-Background", constraintBackground);

    // Cells
    const int32_t startColumn = columnsCounter;
    const int32_t endColumn = columnsCounter + constraintColumns;
    std::string constraintCells;
    for (int32_t i = 0; i < rows; i++) {
      for (int32_t j = startColumn; j < endColumn; j++) {
        const int32_t value = matrix[i][j];
        if (value >= 0) {
          const double posX = originX + constraintCounter * constraintSeparation + j * dlxCellSize;
          const double posY = originY + i * dlxCellSize;
          constraintCells += paperUnitsRect(posX, posY, dlxCellSize, dlxCellSize);
        }
      }
    }
    constraintCells = createGroup("DLX-" + name + "-Cells", constraintCells, darkRectStyle);

    result += constraintText + constraintBackground + constraintCells;

    columnsCounter += constraintColumns;
    constraintCounter++;
  }

  // Horizontal lines
  double lineThickness = boardSize / 5000.0;
  std::string horizondalLines;
  double startX = originX;
  double endX = originX + columnsCounter * dlxCellSize + (constraintCounter - 1) * constraintSeparation;
  for (int32_t i = 0; i <= rows; i++) {
    if (i % 9 == 0) {
      const double y = originY + dlxCellSize * i;
      horizondalLines += paperUnitsLine(startX, y, endX, y);
    }
  }
  result += createGroup("DLX-Horizontal-Lines", horizondalLines, getNoFillStroke(lineThickness));

  // Vertical lines
  std::string verticalLines;
  double startY = originY;
  double endY = originY + rows * dlxCellSize;
  for (int32_t i = 0; i <= columnsAmount + (constraintCounter - 2) * constraintSeparation; i++) {
    if (i % 9 == 0) {
      const double x = originX + dlxCellSize * i;
      verticalLines += paperUnitsLine(x, startY, x, endY);
    }
  }
  result += createGroup("DLX-Vertical-Lines", verticalLines, getNoFillStroke(lineThickness));

  return result;
}

std::string SvgUtilities::toString(double input) {
  constexpr int decimals = 2;
  std::stringstream s;
  s << std::fixed << std::setprecision(decimals) << input;
  return s.str();
}

std::string SvgUtilities::getFontSize(int fontSize) {
  return " font-size=\"" + std::to_string(fontSize) + "\"";
}

std::string SvgUtilities::paperUnitsRect(double x, double y, double width, double height, const std::string& style) {
  return "<rect x=\"" + toString(x) + "\" y=\"" + toString(y) + "\" width=\"" + toString(width) + "\" height=\"" +
         toString(height) + "\"" + style + "/>\n";
}

std::string SvgUtilities::paperUnitsLine(double x1, double y1, double x2, double y2, const std::string& style) {
  return "<line x1=\"" + toString(x1) + "\" y1=\"" + toString(y1) + "\" x2=\"" + toString(x2) + "\" y2=\"" +
         toString(y2) + "\"" + style + "/>\n";
}

std::string SvgUtilities::getRotatedTextStyle(double x, double y, int32_t fontSize) {
  return getFontSize(fontSize) + " text-anchor=\"start\" dominant-baseline=\"central\" transform=\"rotate(-90, " +
         toString(x) + ", " + toString(y) + ")\"";
}

std::string SvgUtilities::getNoFillStroke(double strokeWidth) {
  return " fill-opacity=\"0\" style=\"stroke-width:" + toString(strokeWidth) + "; stroke:" + darkGrey + "\"";
}
