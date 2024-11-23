#pragma once

#include "SvgElement.hpp"

#include <optional>

/** SVG Line
 */
class SvgSquigglyLine : public SvgElement {
public:
  /** Constructor
   * @param x1 The start X-coordinate of the squiggly line
   * @param y1 The start Y-coordinate of the squiggly line
   * @param x2 The end X-coordinate of the squiggly line
   * @param y2 The end Y-coordinate of the squiggly line
   * @param size The size of the squiggles
   * @param stroke An optional stroke color
   * @param strokeWidth An optional stroke width
   */
  SvgSquigglyLine(double x1,
                  double y1,
                  double x2,
                  double y2,
                  double size,
                  const std::optional<const std::string>& stroke,
                  const std::optional<double>& strokeWidth);

  /** Constructor
   * @param x1 The start X-coordinate of the line
   * @param y1 The start Y-coordinate of the line
   * @param x2 The end X-coordinate of the line
   * @param y2 The end Y-coordinate of the line
   * @param size The size of the squiggles
   */
  SvgSquigglyLine(double x1, double y1, double x2, double y2, double size);

  virtual std::string string() const override;

private:
  const double x1 = 0;
  const double y1 = 0;
  const double x2 = 0;
  const double y2 = 0;
  const double size = 0;
  const std::optional<std::string> stroke;
  const std::optional<double> strokeWidth;
};