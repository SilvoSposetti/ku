#pragma once
#include "../utilities/Utilities.h"

#include <bits/floatn-common.h>
#include <cstdint>

const double boardSize = 1000;
const double cellSize = boardSize / static_cast<double>(MAX_DIGIT);
const double boardMargin = cellSize * 1.5;
const double totalBoardSize = boardSize + 2 * boardMargin;
const double infoWidth = 0.5 * totalBoardSize;
const int titleFontSize = 1000 / 25;
const int descriptionFontSize = 1000 / 30;
const int givenDigitFontSize = 1000 / 13;

const std::string strokeColor = "stroke:rgb(0,0,0)";
const std::string thickLine = std::to_string(7);
const std::string mediumLine = std::to_string(4);
const std::string thinLine = std::to_string(2);
const std::string noFillThickStroke =
    " fill-opacity=\"0\" style=\"stroke-width:" + thickLine + "; " + strokeColor + "\"?";
const std::string noFillMediumStroke =
    " fill-opacity=\"0\" style=\"stroke-width:" + mediumLine + "; " + strokeColor + "\"?";
const std::string noFillThinStroke =
    " fill-opacity=\"0\" style=\"stroke-width:" + thinLine + "; " + strokeColor + "\"?";
const std::string centeredTextStyle = " text-anchor=\"middle\" dominant-baseline=\"central\"";
const std::string givenDigitTextStyle =
    " text-anchor=\"middle\" dominant-baseline=\"central\" font-size=\"" + std::to_string(givenDigitFontSize) + "\"";

class SvgUtilities {
public:
  static std::string getSvgHeader();

  static std::string getSvgFooter();

  static std::string createGroup(const std::string& name, const std::string& group, const std::string& style = "");

  static std::string rect(double x, double y, double width, double height, const std::string& style = "");

  static std::string line(double x1, double y1, double x2, double y2, const std::string& style = "");

  static std::string text(double x, double y, const std::string& text, const std::string& style = "");

  static std::string sudokuTitle(const std::string& name);

  static std::string sudokuDescription(int32_t line, const std::string& description);

  static std::string givenDigit(int32_t cellIndexX, int32_t cellIndexY, Sudo digit);

  static std::string getFontSize(int fontSize);

private:
  static std::string toString(double input);
};
