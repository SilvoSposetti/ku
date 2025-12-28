#pragma once

#include "SvgElement.hpp"

#include <optional>

/** SVG Polyline
 */
class SvgPolyline : public SvgElement {
public:
  /** Constructor
   * @param points The list of XY-coordinates that makes up the polyline
   * @param close Whether the polyline should be closed and for a loop (connect first and last points)
   * @param fill An optional fill color
   * @param stroke An optional stroke color
   * @param strokeWidth An optional stroke width
   */
  SvgPolyline(const std::vector<std::pair<double, double>>& points,
              bool closeLoop,
              const std::optional<const std::string>& fill,
              const std::optional<const std::string>& stroke,
              const std::optional<double>& strokeWidth);

  /** Constructor
   * @param points The list of XY-coordinates that makes up the polyline
   * @param close Whether the polyline should be closed and for a loop (connect first and last points)
   */
  SvgPolyline(const std::vector<std::pair<double, double>>& points, bool closeLoop);

  virtual std::string string() const override;

private:
  const std::vector<std::pair<double, double>> points;
  const bool closeLoop = false;
  const std::optional<const std::string> fill;
  const std::optional<const std::string> stroke;
  const std::optional<double> strokeWidth;
};
