#pragma once

#include "SvgElement.h"

#include <optional>

enum class TextAnchor { Start, Middle, End };
enum class TextBaseline { Bottom, Central, Hanging };

/** SVG Text
 */
class SvgText : public SvgElement {
public:
  /** Constructor
   * @param x The X-coordinate of the text
   * @param y The Y-coordinate of the text
   * @param text The string containing the text
   * @param fontSize An optional size
   * @param anchor An optional anchor
   * @param baseline An optional baseline
   * @param fill An optional fill color
   * @param rotationAngle An optional rotation angle in degrees, rotating around the (anchor/baseline) point
   */
  SvgText(double x,
          double y,
          const std::string& text,
          const std::optional<double>& fontSize,
          const std::optional<TextAnchor>& anchor,
          const std::optional<TextBaseline>& baseline,
          const std::optional<std::string>& fill,
          const std::optional<double>& rotationAngle);
  /** Constructor
   * @param x The X-coordinate of the text
   * @param y The Y-coordinate of the text
   * @param text The string containing the text
   * @param fontSize An optional size
   * @param anchor An optional anchor
   * @param baseline An optional baseline
   */
  SvgText(double x,
          double y,
          const std::string& text,
          const std::optional<double>& fontSize,
          const std::optional<TextAnchor>& anchor,
          const std::optional<TextBaseline>& baseline);

  virtual std::string string() const override;

private:
  const double x = 0;
  const double y = 0;
  const std::string text;
  const std::optional<double> fontSize;
  const std::optional<TextAnchor> anchor;
  const std::optional<TextBaseline> baseline;
  const std::optional<const std::string> fill;
  const std::optional<double> rotationAngle;
};