#include "SvgGroup.h"

#include "DrawingUtilities.h"

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
  std::string result = "<" + tagName + " id=\"" + name + "\"";
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
  result += ">\n";
  for (const auto& childElement : childElements) {
    result += childElement->string() + "\n";
  }
  result += "</" + tagName + ">";
  return result;
}