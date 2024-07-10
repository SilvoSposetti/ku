#include "Circle.h"

Circle::Circle(double cx,
               double cy,
               double r,
               const std::optional<const std::string>& fill,
               const std::optional<const std::string>& stroke,
               const std::optional<double>& strokeWidth)
    : Element("circle", false)
    , cx(cx)
    , cy(cy)
    , r(r)
    , fill(fill)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

std::string Circle::string() const {
  std::string result = "<" + tagName + " cx=\"" + number(cx) + "\" cy=\"" + number(cy) + "\" r=\"" + number(r) + "\"";
  if (fill) {
    result += " ";
    result += "fill=\"" + fill.value() + "\"";
  }
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