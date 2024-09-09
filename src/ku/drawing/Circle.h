#pragma once

#include "SvgElement.h"

#include <optional>

/** SVG Circle
 */
class Circle : public SvgElement {
public:
  /** Constructor
   * @param cx The x-coordinate of the circle's center
   * @param cy The y-coordinate of the circle's center
   * @param r The radius of the circle
   * @param fill An optional fill color
   * @param stroke An optional stroke color
   * @param strokeWidth An optional stroke width
   */
  Circle(double cx,
         double cy,
         double r,
         const std::optional<const std::string>& fill,
         const std::optional<const std::string>& stroke,
         const std::optional<double>& strokeWidth);

  virtual std::string string() const override;

private:
  const double cx = 0;
  const double cy = 0;
  const double r = 0;
  const std::optional<const std::string> fill;
  const std::optional<const std::string> stroke;
  const std::optional<double> strokeWidth;
};