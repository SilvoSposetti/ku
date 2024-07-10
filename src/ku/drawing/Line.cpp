#include "Line.h"

Line::Line(double x1,
           double y1,
           double x2,
           double y2,
           const std::optional<const std::string>& stroke,
           const std::optional<double>& strokeWidth)
    : Element("line", false)
    , x1(x1)
    , y1(y1)
    , x2(x2)
    , y2(y2)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

std::string Line::string() const {
  std::string result = "<" + tagName + " x1=\"" + number(x1) + "\" y1=\"" + number(y1) + "\" x2=\"" + number(x2) +
                       "\" y2=\"" + number(y2) + "\"";
  if (stroke) {
    result += " ";
    result += "stroke=\"" + stroke.value() + "\"";
  }
  if (strokeWidth) {
    result += " ";
    result += "stroke-width=\"" + number(strokeWidth.value()) + "\"";
  }
  result += "/>";
  return result;
}