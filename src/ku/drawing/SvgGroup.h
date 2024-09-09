#pragma once

#include "SvgElement.h"

#include <optional>

/** SVG group
 */
class SvgGroup : public SvgElement {
public:
  /** Constructor
   * @param name The name of the group
   * @param fill An optional fill color
   * @param stroke An optional stroke color
   * @param strokeWidth An optional stroke width
   */
  SvgGroup(const std::string& name,
        const std::optional<const std::string>& fill,
        const std::optional<const std::string>& stroke,
        const std::optional<double>& strokeWidth);

  virtual std::string string() const override;

private:
  const std::string name;
  const std::optional<const std::string> fill;
  const std::optional<const std::string> stroke;
  const std::optional<double> strokeWidth;
};