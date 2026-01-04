#include "SvgLine.hpp"

#include "DrawingUtilities.hpp"

SvgLine::SvgLine(double x1,
                 double y1,
                 double x2,
                 double y2,
                 const std::optional<const std::string>& stroke,
                 const std::optional<double>& strokeWidth)
    : SvgElement("line", false)
    , x1(x1)
    , y1(y1)
    , x2(x2)
    , y2(y2)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

SvgLine::SvgLine(double x1, double y1, double x2, double y2)
    : SvgLine(x1, y1, x2, y2, std::nullopt, std::nullopt) {}

std::string SvgLine::string() const {
  std::string result = "<" + tagName + " x1=\"" + DrawingUtilities::number(x1) + "\" y1=\"" +
                       DrawingUtilities::number(y1) + "\" x2=\"" + DrawingUtilities::number(x2) + "\" y2=\"" +
                       DrawingUtilities::number(y2) + "\"";
  if (stroke) {
    result += " ";
    result += "stroke=\"" + stroke.value() + "\"";
  }
  if (strokeWidth) {
    result += " ";
    result += "stroke-width=\"" + DrawingUtilities::number(strokeWidth.value()) + "\"";
  }
  result += "/>";
  return result;
}
