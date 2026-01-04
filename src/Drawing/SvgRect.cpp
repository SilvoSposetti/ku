#include "SvgRect.hpp"

#include "DrawingUtilities.hpp"

SvgRect::SvgRect(double x,
                 double y,
                 double width,
                 double height,
                 const std::optional<const std::string>& fill,
                 const std::optional<const std::string>& stroke,
                 const std::optional<double>& strokeWidth)
    : SvgElement("rect", false)
    , x(x)
    , y(y)
    , width(width)
    , height(height)
    , fill(fill)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

SvgRect::SvgRect(double x, double y, double width, double height)
    : SvgRect(x, y, width, height, std::nullopt, std::nullopt, std::nullopt) {}

std::string SvgRect::string() const {
  std::string result = std::format("<{} x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\"",
                                   tagName,
                                   DrawingUtilities::number(x),
                                   DrawingUtilities::number(y),
                                   DrawingUtilities::number(width),
                                   DrawingUtilities::number(height));
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
