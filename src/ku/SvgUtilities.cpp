#include "SvgUtilities.h"

#include <algorithm>
#include <cmath>
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

std::string SvgUtilities::polyLine(const std::vector<std::pair<double, double>>& points, const std::string& style) {
  std::string pointsList;
  for (const auto& point : points) {

    pointsList += toString(point.first * boardSize) + "," + toString(point.second * boardSize);
    if (&point != &points.back()) {
      pointsList += " ";
    }
  }
  return "<polyline points=\"" + pointsList + "\"" + style + "/>\n";
}

std::string SvgUtilities::rect(double x, double y, double width, double height, const std::string& style) {
  const double xPosition = x * boardSize;
  const double yPosition = y * boardSize;
  const double rectWidth = width * boardSize;
  const double rectHeight = height * boardSize;

  return "<rect x=\"" + toString(xPosition) + "\" y=\"" + toString(yPosition) + "\" width=\"" + toString(rectWidth) +
         "\" height=\"" + toString(rectHeight) + "\"" + style + "/>\n";
}

std::string SvgUtilities::rotatedRect(
    double x, double y, double width, double height, double rotationAngle, const std::string& style) {

  const double TLX = -width * .5;
  const double TLY = -height * .5;
  const double TRX = width * .5;
  const double TRY = -height * .5;
  const double BLX = -width * .5;
  const double BLY = height * .5;
  const double BRX = width * .5;
  const double BRY = height * .5;

  const double c = cos(rotationAngle);
  const double s = sin(rotationAngle);
  const double topLeftX = ((TLX * c - TLY * s) + x) * boardSize;
  const double topLeftY = ((TLX * s + TLY * c) + y) * boardSize;
  const double topRightX = ((TRX * c - TRY * s) + x) * boardSize;
  const double topRightY = ((TRX * s + TRY * c) + y) * boardSize;
  const double bottomLeftX = ((BLX * c - BLY * s) + x) * boardSize;
  const double bottomLeftY = ((BLX * s + BLY * c) + y) * boardSize;
  const double bottomRightX = ((BRX * c - BRY * s) + x) * boardSize;
  const double bottomRightY = ((BRX * s + BRY * c) + y) * boardSize;

  return "<polygon points=\"" + toString(topLeftX) + "," + toString(topLeftY) + " " + toString(topRightX) + "," +
         toString(topRightY) + " " + toString(bottomRightX) + "," + toString(bottomRightY) + " " +
         toString(bottomLeftX) + "," + toString(bottomLeftY) + "\" " + style + "/>\n";
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

std::string SvgUtilities::givenDigits(const std::vector<std::vector<Sudo::Digit>>& solution,
                                      const std::vector<std::vector<bool>>& givenMask) {
  const int givenDigitFontSize = boardSize / 13;
  const std::string givenDigitTextStyle =
      " text-anchor=\"middle\" dominant-baseline=\"central\" font-size=\"" + std::to_string(givenDigitFontSize) + "\"";

  std::string givenDigits;
  for (const auto& i : Sudo::INDICES) {
    for (const auto& j : Sudo::INDICES) {
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
  for (const auto& i : Sudo::INDICES) {
    for (const auto& j : Sudo::INDICES) {
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
  const double y = boardSize + boardMargin - givenPatternCellSize * ((Sudo::MAX_DIGIT + 2) - cellIndexI);
  const std::string style = isGiven ? darkRectStyle : lightRectStyle;
  return paperUnitsRect(x, y, givenPatternCellSize, givenPatternCellSize, style);
}

std::string SvgUtilities::givenPatternBorder() {
  const double x = boardSize + boardMargin;
  const double y = boardSize + boardMargin - givenPatternCellSize * ((Sudo::MAX_DIGIT + 2));
  const double size = givenPatternCellSize * Sudo::MAX_DIGIT;
  return paperUnitsRect(x, y, size, size, getNoFillStroke(thinLine));
}

std::string SvgUtilities::dlxMatrix(const SparseCoordinateMatrix& matrix,
                                    const std::vector<std::pair<std::string, std::vector<bool>>>& constraintsInfo) {

  int32_t columnsAmount = matrix.getColumnsAmount();
  const int32_t rowsAmount = matrix.getRowsAmount();

  const double originX = 0;
  const double originY = 0;

  const int32_t constraintSeparationAmount = std::max(0.0, static_cast<double>(constraintsInfo.size() - 1));
  const double constraintSeparationCellMultiplier = static_cast<double>(Sudo::MAX_DIGIT);

  const double verticalCellSize = boardSize / rowsAmount;
  const double horizontalCellSize =
      boardSize / (columnsAmount + constraintSeparationAmount * constraintSeparationCellMultiplier);
  const double dlxCellSize = std::min(verticalCellSize, horizontalCellSize);

  const int32_t textSize = boardSize / 100;
  const int32_t textDistance = dlxCellSize * Sudo::MAX_DIGIT * 2;
  const double constraintSeparation = dlxCellSize * constraintSeparationCellMultiplier;

  const double namesBuffer = boardSize * 0.15;
  const double actualHeight = dlxCellSize * rowsAmount + namesBuffer;
  const double actualWidth = dlxCellSize * columnsAmount + constraintSeparation * constraintSeparationAmount;

  // Keep track of which columns have already been considered
  int32_t columnsCounter = 0;
  int32_t constraintCounter = 0;

  std::string names;
  std::string backgrounds;
  std::string optionalConstraintsBackgrounds;
  std::string cells;
  for (const auto& constraintInfo : constraintsInfo) {
    const double constraintOriginX = originX + columnsCounter * dlxCellSize + constraintCounter * constraintSeparation;
    const std::string name = constraintInfo.first;
    const int32_t constraintColumns = constraintInfo.second.size();

    // Name
    double textPositionX = constraintOriginX + (constraintColumns * 0.5) * dlxCellSize;
    double textPositionY = originY + rowsAmount * dlxCellSize + textDistance;
    std::string constraintText =
        text(textPositionX, textPositionY, name, getRotatedTextStyle(textPositionX, textPositionY, textSize));

    // Background
    std::string constraintBackground = paperUnitsRect(
        constraintOriginX, originY, dlxCellSize * constraintColumns, dlxCellSize * rowsAmount, whiteRectStyle);

    // Optional constraints
    int32_t optionalColumnsCounter = 0;
    std::string optionalConstraintsBackground;
    double boardHeight = dlxCellSize * rowsAmount;

    if (std::all_of(constraintInfo.second.begin(), constraintInfo.second.end(), [](bool v) { return !v; })) {
      // All columns are optional for this constraint. Do one big rectangle for this constraint
      optionalConstraintsBackground +=
          paperUnitsRect(constraintOriginX, boardHeight, constraintColumns * dlxCellSize, textDistance);
    } else {
      // Some columns are optionals, some are not. Do multiple small rectangle to show which ones
      for (const auto& isColumnPrimary : constraintInfo.second) {
        if (!isColumnPrimary) {
          double leftX = (columnsCounter + optionalColumnsCounter + constraintCounter * Sudo::MAX_DIGIT) * dlxCellSize;
          double rightX = dlxCellSize;
          optionalConstraintsBackground += paperUnitsRect(leftX, boardHeight, rightX, textDistance);
        }
        optionalColumnsCounter++;
      }
    }

    optionalConstraintsBackground =
        createGroup("DLX-" + name + "-Optional-Constraints", optionalConstraintsBackground, middleGreyRectStyle);

    // Cells
    const int32_t startColumn = columnsCounter;
    const int32_t endColumn = columnsCounter + constraintColumns;
    std::string constraintCells;
    for (int32_t i = 0; i < rowsAmount; i++) {
      for (int32_t j = startColumn; j < endColumn; j++) {
        if (matrix.isCellSet(i, j)) {
          const double posX = originX + constraintCounter * constraintSeparation + j * dlxCellSize;
          const double posY = originY + i * dlxCellSize;
          constraintCells += paperUnitsRect(posX, posY, dlxCellSize, dlxCellSize);
        }
      }
    }
    constraintCells = createGroup("DLX-" + name + "-Cells", constraintCells, darkRectStyle);

    names += constraintText;
    backgrounds += constraintBackground;
    optionalConstraintsBackgrounds += optionalConstraintsBackground;
    cells += constraintCells;
    columnsCounter += constraintColumns;
    constraintCounter++;
  }
  names = createGroup("DLX-Names", names);
  backgrounds = createGroup("DLX-Backgrounds", backgrounds, whiteRectStyle);
  optionalConstraintsBackgrounds =
      createGroup("DLX-OptionalConstraintsBackgrounds", optionalConstraintsBackgrounds, whiteRectStyle);
  cells = createGroup("DLX-Cells", cells, darkRectStyle);

  // Grid
  double lineThickness = dlxCellSize / 5.0;
  // Horizontal lines
  std::string horizontalLines;
  double startX = originX;
  double endX = actualWidth;
  int32_t currentColumn = 0;

  for (int32_t i = 0; i < rowsAmount; i++) {
    if (matrix.isCellSet(i, currentColumn) && currentColumn < columnsAmount) {
      const double y = originY + dlxCellSize * i;
      horizontalLines += paperUnitsLine(startX, y, endX, y);
      currentColumn++;
    }
  }
  horizontalLines += paperUnitsLine(startX, dlxCellSize * rowsAmount, endX, dlxCellSize * rowsAmount);
  horizontalLines = createGroup("DLX-Horizontal-Lines", horizontalLines, getNoFillStroke(lineThickness));

  // Vertical lines
  std::string verticalLines;
  int32_t verticalLinesAmount = matrix.getColumnsAmount() + constraintCounter * Sudo::MAX_DIGIT - 1;
  double startY = originY;
  double endY = originY + rowsAmount * dlxCellSize;
  for (int32_t i = 0; i <= verticalLinesAmount; i++) {
    if (i % Sudo::MAX_DIGIT == 0) {
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

  return header + background + names + backgrounds + optionalConstraintsBackgrounds + cells + horizontalLines +
         verticalLines + footer;
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

std::string SvgUtilities::getNoFillDashedStroke(double strokeWidth, int lineLength, int lineSpacing) {
  return " fill-opacity=\"0\" style=\"stroke-width:" + toString(strokeWidth) + "; stroke:" + darkGrey +
         "\" stroke-dasharray=\"" + std::to_string(lineLength) + " " + std::to_string(lineSpacing) + "\"";
}

std::string SvgUtilities::getFill(std::string color) {
  return " style=\"fill:" + color + ";\"";
}

std::string SvgUtilities::plus(double x, double y, double size) {

  return line(x - size, y, x + size, y) + line(x, y - size, x, y + size);
}

std::string SvgUtilities::zigZagLine(double x1, double y1, double x2, double y2, double size) {
  double length = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
  int32_t segmentsAmount = static_cast<int>(std::floor(length / size));

  double lineDirectionX = (x2 - x1) / length;
  double lineDirectionY = (y2 - y1) / length;
  double perpendicularDirectionX = lineDirectionY;
  double perpendicularDirectionY = -lineDirectionX;

  segmentsAmount = std::max(segmentsAmount, 1); // At least one segment
  double actualSize = length / static_cast<double>(segmentsAmount);

  std::vector<std::pair<double, double>> points(segmentsAmount + 1);
  for (int32_t i = 0; i <= segmentsAmount; ++i) {
    const int32_t parity = i % 2 == 0 ? 1 : -1;
    const double offset = parity * actualSize * 0.5;
    const double pointX = x1 + i * actualSize * lineDirectionX + offset * perpendicularDirectionX;
    const double pointY = y1 + i * actualSize * lineDirectionY + offset * perpendicularDirectionY;
    points[i] = {pointX, pointY};
  }
  return polyLine(points);
}

std::string SvgUtilities::squigglyLine(double x1, double y1, double x2, double y2, double size) {
  double length = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
  int32_t segmentsAmount = static_cast<int>(std::floor(length / size));
  segmentsAmount = std::max(segmentsAmount, 1); // At least one segment
  double actualSize = length / static_cast<double>(segmentsAmount);
  const double controlOffsetLength = actualSize / 2;

  double lineDirectionX = (x2 - x1) / length;
  double lineDirectionY = (y2 - y1) / length;
  double perpendicularDirectionX = lineDirectionY;
  double perpendicularDirectionY = -lineDirectionX;

  // Create squiggly line given set of vertices
  const double controlOffsetX = controlOffsetLength * lineDirectionX;
  const double controlOffsetY = controlOffsetLength * lineDirectionY;

  double side = 1;

  std::string squiggly = "<path d=\"M " + getPointString({x1, y1});
  for (int32_t i = 1; i < segmentsAmount; i++) {
    const double offset = side * actualSize * 0.5;
    const double pointX = x1 + i * actualSize * lineDirectionX + offset * perpendicularDirectionX;
    const double pointY = y1 + i * actualSize * lineDirectionY + offset * perpendicularDirectionY;

    const double controlX = pointX - controlOffsetX;
    const double controlY = pointY - controlOffsetY;

    squiggly += " S " + getPointString({controlX * boardSize, controlY * boardSize}) + " " +
                getPointString({pointX * boardSize, pointY * boardSize});
    side = -side;
  }
  squiggly += "\"/>";
  return squiggly;
}

std::string SvgUtilities::getPointString(std::pair<double, double> point) {
  return toString(point.first) + "," + toString(point.second);
}
