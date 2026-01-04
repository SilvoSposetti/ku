#include "SvgCircle.hpp"

#include "DrawingUtilities.hpp"

#include <format>

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
  std::string result = std::format("<{} cx=\"{}\" cy=\"{}\" r=\"{}\"",
                                   tagName,
                                   DrawingUtilities::number(cx),
                                   DrawingUtilities::number(cy),
                                   DrawingUtilities::number(r));
  if (fill) {
    result += std::format(" fill=\"{}\"", fill.value());
  }
  if (stroke) {
    result += std::format(" stroke=\"{}\"", stroke.value());
  }
  if (strokeWidth) {
    result += std::format(" stroke-width=\"{}\"", DrawingUtilities::number(strokeWidth.value()));
  }
  result += "/>";
  return result;
}
