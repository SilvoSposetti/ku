#include "Rect.h"

Rect::Rect(double x,
           double y,
           double width,
           double height,
           const std::optional<const std::string>& fill,
           const std::optional<const std::string>& stroke,
           const std::optional<double>& strokeWidth)
    : Element("rect", false)
    , x(x)
    , y(y)
    , width(width)
    , height(height)
    , fill(fill)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

std::string Rect::string() const {
  std::string result = "<" + tagName + " x=\"" + number(x) + "\" y=\"" + number(y) + "\" width=\"" + number(width) +
                       "\" height=\"" + number(height) + "\"";
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