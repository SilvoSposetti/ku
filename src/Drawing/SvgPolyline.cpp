#include "SvgPolyline.hpp"

#include "DrawingUtilities.hpp"
#include "StringUtilities.hpp"

#include <format>

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

SvgPolyline::SvgPolyline(const std::vector<std::pair<double, double>>& points, bool closeLoop)
    : SvgPolyline(points, closeLoop, std::nullopt, std::nullopt, std::nullopt) {}

std::string SvgPolyline::string() const {

  std::string polylineString;
  for (const auto& point : points) {
    polylineString += DrawingUtilities::getPointString(point);
    if (&point != &points.back()) {
      polylineString += " ";
    }
  }

  if (closeLoop && !points.empty()) {
    const auto& firstPoint = points.front();
    polylineString += " " + DrawingUtilities::getPointString(firstPoint);
  }

  // Switch to polygon element if polyline needs to be closed
  const std::string name = closeLoop ? "polygon" : tagName;

  std::string result = std::format("<{} points=\"{}\"", name, polylineString);
  if (fill) {
    result += std::format(" fill=\"{}\"", fill.value());
  }
  if (stroke) {
    result += std::format(" stroke=\"{}\"", stroke.value());
  }
  if (strokeWidth) {
    result += std::format(" stroke-width=\"{}\"", StringUtilities::number(strokeWidth.value()));
  }
  result += "/>";
  return result;
}
