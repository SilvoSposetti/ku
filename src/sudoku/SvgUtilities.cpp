#include "SvgUtilities.h"

#include <iomanip>

std::string SvgUtilities::getSvgHeader() {
  std::string header = "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"";

  // ViewBox
  header += toString(-boardMargin) + " " + toString(-boardMargin) + " " + toString(totalBoardSize) + " " +
            toString(totalBoardSize + infoHeight) + "\" font-family=\"Open Sans\" >\n";

  return header;
}
std::string SvgUtilities::getSvgFooter() {
  return "</svg>\n";
  ;
}

std::string SvgUtilities::background() {
  return "<rect x=\"" + toString(-boardMargin) + "\" y=\"" + toString(-boardMargin) + "\" width=\"" +
         toString(totalBoardSize) + "\" height=\"" + toString(totalBoardSize + infoHeight) + "\"" + lightRectStyle +
         "/>\n";
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
                                    const std::vector<std::pair<std::string, int32_t>>& constraintNamesAndColumns) {

  int32_t columnsAmount = 0;
  for (const auto& columnNameAndAmount : constraintNamesAndColumns) {
    columnsAmount += columnNameAndAmount.second;
  }
  const int32_t rowsAmount = matrix.size();

  const double originX = 0;
  const double originY = 0;

  const int32_t constraintSeparationAmount = std::max(0.0, static_cast<double>(constraintNamesAndColumns.size() - 1));
  const double constraintSeparationCellMultiplier = 9.0;

  const double verticalCellSize = boardSize / rowsAmount;
  const double horizontalCellSize =
      boardSize / (columnsAmount + constraintSeparationAmount * constraintSeparationCellMultiplier);
  const double dlxCellSize = std::min(verticalCellSize, horizontalCellSize);

  const int32_t textSize = boardSize / 100;
  const int32_t textDistance = dlxCellSize * 5;
  const double constraintSeparation = dlxCellSize * constraintSeparationCellMultiplier;

  const double namesBuffer = boardSize * 0.15;
  const double actualHeight = dlxCellSize * rowsAmount + namesBuffer;
  const double actualWidth = dlxCellSize * columnsAmount + constraintSeparation * constraintSeparationAmount;

  // Keep track of which columns have already been considered
  int32_t columnsCounter = 0;
  int32_t constraintCounter = 0;

  std::string names;
  std::string backgrounds;
  std::string cells;
  for (const auto& columnNameAndAmount : constraintNamesAndColumns) {
    const double constraintOriginX = originX + columnsCounter * dlxCellSize + constraintCounter * constraintSeparation;
    const std::string name = columnNameAndAmount.first;
    const int32_t constraintColumns = columnNameAndAmount.second;

    // Name
    double textPositionX = constraintOriginX + (constraintColumns * 0.5) * dlxCellSize;
    double textPositionY = originY + rowsAmount * dlxCellSize + textDistance;
    std::string constraintText =
        text(textPositionX, textPositionY, name, getRotatedTextStyle(textPositionX, textPositionY, textSize));

    // Background
    std::string constraintBackground = paperUnitsRect(
        constraintOriginX, originY, dlxCellSize * constraintColumns, dlxCellSize * rowsAmount, whiteRectStyle);

    // Cells
    const int32_t startColumn = columnsCounter;
    const int32_t endColumn = columnsCounter + constraintColumns;
    std::string constraintCells;
    for (int32_t i = 0; i < rowsAmount; i++) {
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

    names += constraintText;
    backgrounds += constraintBackground;
    cells += constraintCells;
    columnsCounter += constraintColumns;
    constraintCounter++;
  }
  names = createGroup("DLX-Names", names);
  backgrounds = createGroup("DLX-Backgrounds", backgrounds, whiteRectStyle);
  cells = createGroup("DLX-Cells", cells, darkRectStyle);

  // Horizontal lines
  double lineThickness = boardSize / 5000.0;
  std::string horizontalLines;
  double startX = originX;
  double endX = actualWidth;
  int32_t currentColumn = 0;

  for (int32_t i = 0; i < rowsAmount; i++) {
    if (matrix[i][currentColumn] >= 0) {
      const double y = originY + dlxCellSize * i;
      horizontalLines += paperUnitsLine(startX, y, endX, y);
      currentColumn++;
    }
  }
  horizontalLines += paperUnitsLine(startX, dlxCellSize * rowsAmount, endX, dlxCellSize * rowsAmount);
  horizontalLines = createGroup("DLX-Horizontal-Lines", horizontalLines, getNoFillStroke(lineThickness));

  // Vertical lines
  std::string verticalLines;
  double startY = originY;
  double endY = originY + rowsAmount * dlxCellSize;
  for (int32_t i = 0; i <= columnsAmount + (constraintCounter - 2) * constraintSeparation; i++) {
    if (i % 9 == 0) {
      const double x = originX + dlxCellSize * i;
      verticalLines += paperUnitsLine(x, startY, x, endY);
    }
  }
  verticalLines = createGroup("DLX-Vertical-Lines", verticalLines, getNoFillStroke(lineThickness));

  // Header
  const std::string header = "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"" + toString(0) + " " + toString(0) +
                             " " + toString(actualWidth) + " " + toString(actualHeight) + "\" >\n";

  const std::string background = paperUnitsRect(0, 0, actualWidth, actualHeight, lightRectStyle);
  // Footer
  const std::string footer = getSvgFooter();

  return header + background + names + backgrounds + cells + horizontalLines + verticalLines + footer;
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
  return getFontSize(fontSize) + " text-anchor=\"start\" dominant-baseline=\"central\" transform=\"rotate(90, " +
         toString(x) + ", " + toString(y) + ")\"";
}

std::string SvgUtilities::getNoFillStroke(double strokeWidth) {
  return " fill-opacity=\"0\" style=\"stroke-width:" + toString(strokeWidth) + "; stroke:" + darkGrey + "\"";
}
