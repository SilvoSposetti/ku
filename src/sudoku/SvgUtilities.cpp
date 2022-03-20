#include "SvgUtilities.h"

#include <cstdint>
#include <iomanip>

std::string SvgUtilities::getSvgHeader() {
  std::string header = "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"";

  // ViewBox
  header += toString(-boardMargin) + " " + toString(-boardMargin) + " " + toString(totalBoardSize + infoWidth) + " " +
            toString(totalBoardSize) + "\">\n";

  return header;
}
std::string SvgUtilities::getSvgFooter() {
  return "</svg>\n";
  ;
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

std::string SvgUtilities::sudokuTitle(const std::string& name) {
  return text(boardSize + boardMargin, 0, name, getFontSize(titleFontSize));
}

std::string SvgUtilities::sudokuDescription(int32_t line, const std::string& description) {
  return text(boardSize + boardMargin, 70 + line * 50, description, getFontSize(descriptionFontSize));
}

std::string SvgUtilities::givenDigit(int32_t cellIndexI, int32_t cellIndexJ, Sudo digit) {
  const std::string digitString = std::to_string(static_cast<int32_t>(digit));

  const double cellCenterX = (cellIndexJ + 0.5) * cellSize;
  const double cellCenterY = (cellIndexI + 0.5) * cellSize;
  return text(cellCenterX, cellCenterY, digitString);
  // "<text x=\"" + toString(cellCenterX) + "\" y=\"" + toString(cellCenterY) + "\"" + centeredText +
  //  " font-size=\"" + std::to_string(givenDigitFontSize) + "\">" + digitString + "</text>\n";
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