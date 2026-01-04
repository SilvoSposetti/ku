#include "SvgText.hpp"

#include "DrawingUtilities.hpp"

#include <format>

SvgText::SvgText(double x,
                 double y,
                 const std::string& text,
                 const std::optional<double>& fontSize,
                 const std::optional<TextAnchor>& anchor,
                 const std::optional<TextBaseline>& baseline,
                 const std::optional<std::string>& fill,
                 const std::optional<double>& rotationAngle)
    : SvgElement("text", false)
    , x(x)
    , y(y)
    , text(text)
    , fontSize(fontSize)
    , anchor(anchor)
    , baseline(baseline)
    , fill(fill)
    , rotationAngle(rotationAngle) {}

SvgText::SvgText(double x,
                 double y,
                 const std::string& text,
                 const std::optional<double>& fontSize,
                 const std::optional<TextAnchor>& anchor,
                 const std::optional<TextBaseline>& baseline)
    : SvgText(x, y, text, fontSize, anchor, baseline, std::nullopt, std::nullopt) {}

std::string SvgText::string() const {
  std::string result =
      std::format("<{} x=\"{}\" y=\"{}\"", tagName, DrawingUtilities::number(x), DrawingUtilities::number(y));
  if (fontSize) {
    result += std::format(" font-size=\"{}\"", DrawingUtilities::number(fontSize.value()));
  }
  if (anchor) {
    std::string anchorText;
    switch (anchor.value()) {
    default:
    case TextAnchor::Start:
      anchorText = "start";
      break;
    case TextAnchor::Middle:
      anchorText = "middle";
      break;
    case TextAnchor::End:
      anchorText = "end";
      break;
    }
    result += std::format(" text-anchor=\"{}\"", anchorText);
  }
  if (baseline) {
    std::string baselineText;
    switch (baseline.value()) {
    default:
    case TextBaseline::Bottom:
      baselineText = "text-bottom";
      break;
    case TextBaseline::Central:
      baselineText = "central";
      break;
    case TextBaseline::Hanging:
      baselineText = "hanging";
      break;
    }
    result += std::format(" alignment-baseline=\"{}\"", baselineText);
  }
  if (fill) {
    result += std::format(" fill=\"{}\"", fill.value());
  }
  if (rotationAngle) {
    result += std::format(" transform=\"rotate({}, {}, {})\"",
                          DrawingUtilities::number(rotationAngle.value()),
                          DrawingUtilities::number(x),
                          DrawingUtilities::number(y));
  }
  result += std::format(">{}</{}>", text, tagName);
  return result;
}
