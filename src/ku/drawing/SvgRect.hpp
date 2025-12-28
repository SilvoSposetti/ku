#pragma once

#include "SvgElement.hpp"

#include <optional>

/** SVG Rectangle
 */
class SvgRect : public SvgElement {
public:
  /** Constructor
   * @param x The top-left X-coordinate of the rectangle
   * @param y The top-left Y-coordinatge of the rectangle
   * @param width The width of the rectangle
   * @param height The height for the rectangle
   * @param fill An optional fill color
   * @param stroke An optional stroke color
   * @param strokeWidth An optional stroke width
   */
  SvgRect(double x,
          double y,
          double width,
          double height,
          const std::optional<const std::string>& fill,
          const std::optional<const std::string>& stroke,
          const std::optional<double>& strokeWidth);

  /** Constructor
   * @param x The top-left X-coordinate of the rectangle
   * @param y The top-left Y-coordinatge of the rectangle
   * @param width The width of the rectangle
   * @param height The height for the rectangle
   */
  SvgRect(double x, double y, double width, double height);

  virtual std::string string() const override;

private:
  const double x = 0;
  const double y = 0;
  const double width = 0;
  const double height = 0;
  const std::optional<std::string> fill = "none";
  const std::optional<std::string> stroke;
  const std::optional<double> strokeWidth;
};
