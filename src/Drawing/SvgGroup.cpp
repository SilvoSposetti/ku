#include "SvgGroup.hpp"

#include "DrawingUtilities.hpp"

#include <format>

SvgGroup::SvgGroup(const std::string& name,
                   const std::optional<const std::string>& fill,
                   const std::optional<const std::string>& stroke,
                   const std::optional<double>& strokeWidth)
    : SvgElement("g", true)
    , name(name)
    , fill(fill)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

std::string SvgGroup::string() const {
  std::string result = std::format("<{} id=\"{}\"", tagName, name);
  if (fill) {
    result += std::format(" fill=\"{}\"", fill.value());
  }
  if (stroke) {
    result += std::format(" stroke=\"{}\"", stroke.value());
  }
  if (strokeWidth) {
    result += std::format(" stroke-width=\"{}\"", DrawingUtilities::number(strokeWidth.value()));
  }
  result += ">\n";
  for (const auto& childElement : childElements) {
    result += childElement->string() + "\n";
  }
  result += std::format("</{}>", tagName);
  return result;
}
