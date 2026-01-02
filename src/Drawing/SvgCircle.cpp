#include "SvgCircle.hpp"

#include "DrawingUtilities.hpp"

SvgCircle::SvgCircle(double cx,
                     double cy,
                     double r,
                     const std::optional<const std::string>& fill,
                     const std::optional<const std::string>& stroke,
                     const std::optional<double>& strokeWidth)
    : SvgElement("circle", false)
    , cx(cx)
    , cy(cy)
    , r(r)
    , fill(fill)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

SvgCircle::SvgCircle(double cx, double cy, double r)
    : SvgCircle(cx, cy, r, std::nullopt, std::nullopt, std::nullopt) {}

std::string SvgCircle::string() const {
  std::string result = "<" + tagName + " cx=\"" + DrawingUtilities::number(cx) + "\" cy=\"" +
                       DrawingUtilities::number(cy) + "\" r=\"" + DrawingUtilities::number(r) + "\"";
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
    result += "stroke-width=\"" + DrawingUtilities::number(strokeWidth.value()) + "\"";
  }
  result += "/>";
  return result;
}
