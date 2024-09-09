#include "SvgPolyline.h"

SvgPolyline::SvgPolyline(const std::vector<std::pair<double, double>>& points,
                         bool closeLoop,
                         const std::optional<const std::string>& fill,
                         const std::optional<const std::string>& stroke,
                         const std::optional<double>& strokeWidth)
    : SvgElement("polyline", false)
    , points(points)
    , closeLoop(closeLoop)
    , fill(fill)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

std::string SvgPolyline::string() const {

  std::string polylineString;
  for (const auto& point : points) {
    polylineString += number(point.first) + "," + number(point.second);
    if (&point != &points.back()) {
      polylineString += " ";
    }
  }

  if (closeLoop && !points.empty()) {
    const auto& firstPoint = points.front();
    polylineString += " " + number(firstPoint.first) + "," + number(firstPoint.second);
  }

  // Switch to polygon element if polyline needs to be closed
  const std::string name = closeLoop ? "polygon" : tagName;
  std::string result = "<" + name + " points=\"" + polylineString + "\"";
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
