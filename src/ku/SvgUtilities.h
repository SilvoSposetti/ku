#pragma once
#include "../utilities/Utilities.h"

const double boardSize = 1000;
const double cellSize = boardSize / static_cast<double>(MAX_DIGIT);
const double boardMargin = cellSize * 1.5;
const double totalBoardSize = boardSize + 2 * boardMargin;
const double dlxMatrixHeight = boardSize * 1.2;
const double infoHeight = 0.4 * boardSize;

const int givenPatternCellSize = boardSize / 50;

const std::string black = "rgb(0,0,0)";
const std::string darkGrey = "rgb(20,20,20)";
const std::string middleGrey = "rgb(130,130,130)";
const std::string lightGrey = "rgb(230,230,230)";
const std::string white = "rgb(255,255,255)";

const double thickLine = 8;
const double mediumLine = 5;
const double thinLine = 2;
const double thinnestLine = 1;

const std::string centeredTextStyle = " text-anchor=\"middle\" dominant-baseline=\"central\"";

const std::string lightRectStyle = " style=\"fill:" + lightGrey + ";\"";
const std::string darkRectStyle = " style=\"fill:" + darkGrey + ";\"";
const std::string middleGreyRectStyle = " style=\"fill:" + middleGrey + ";\"";
const std::string whiteRectStyle = " style=\"fill:" + white + ";\"";

class SvgUtilities {
public:
  static std::string getSvgHeader();

  static std::string getSvgFooter();

  static std::string background();

  static std::string createGroup(const std::string& name, const std::string& group, const std::string& style = "");

  static std::string rect(double x, double y, double width, double height, const std::string& style = "");

  static std::string
  rotatedRect(double x, double y, double width, double height, double rotationAngle, const std::string& style = "");

  static std::string line(double x1, double y1, double x2, double y2, const std::string& style = "");

  static std::string polyLine(const std::vector<std::pair<double, double>>& points, const std::string& style = "");

  static std::string text(double x, double y, const std::string& text, const std::string& style = "");

  static std::string titleAndDescription(const std::string& sudokuName,
                                         const std::vector<std::string>& constraintDescriptions);

  static std::string givenDigits(const std::vector<std::vector<Sudo>>& solution,
                                 const std::vector<std::vector<bool>>& givenMask);

  static std::string givenDigit(int32_t cellIndexI, int32_t cellIndexJ, Sudo digit);

  static std::string givenPattern(int32_t cellIndexI, int32_t cellIndexJ, bool isGiven);

  static std::string givenPatternBorder();

  static std::string getFontSize(int fontSize);

  static std::string dlxMatrix(const std::vector<std::vector<int32_t>>& dlxMatrix,
                               const std::vector<std::pair<std::string, std::vector<bool>>>& constraintsInfo);

  static std::string getNoFillStroke(double strokeWidth);

  static std::string getNoFillDashedStroke(double strokeWidth, int lineLength = 7, int lineSpacing = 7);

  static std::string getFill(std::string color);

  static std::string plus(double x, double y, double size);

  static std::string zigZagLine(double x1, double y1, double x2, double y2, double size = 0.005);

  static std::string squigglyLine(double x1, double y1, double x2, double y2, double size = 0.005);

private:
  static std::string paperUnitsRect(double x, double y, double width, double height, const std::string& style = "");

  static std::string paperUnitsLine(double x1, double y1, double x2, double y2, const std::string& style = "");

  static std::string toString(double input);

  static std::string getRotatedTextStyle(double x, double y, int32_t fontSize);

  static std::string getPointString(std::pair<double, double> point);
};
