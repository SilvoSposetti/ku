#include "SvgZigZagLine.hpp"

#include "DrawingUtilities.hpp"
#include "StringUtilities.hpp"

#include <cmath>
#include <format>

SvgZigZagLine::SvgZigZagLine(double x1,
                             double y1,
                             double x2,
                             double y2,
                             double size,
                             const std::optional<const std::string>& stroke,
                             const std::optional<double>& strokeWidth)
    : SvgElement("path", false)
    , x1(x1)
    , y1(y1)
    , x2(x2)
    , y2(y2)
    , size(size)
    , stroke(stroke)
    , strokeWidth(strokeWidth) {}

SvgZigZagLine::SvgZigZagLine(double x1, double y1, double x2, double y2, double size)
    : SvgZigZagLine(x1, y1, x2, y2, size, std::nullopt, std::nullopt) {}

std::string SvgZigZagLine::string() const {

  std::string zigZagLineString;
  if (size > 0) {
    const double length = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    const int32_t segmentsAmount = std::max(static_cast<int>(std::floor(length / size)), 1); // At least one segment
    const double actualElementSize = length / static_cast<double>(segmentsAmount);
    const double offset = actualElementSize / 2;

    const double lineDirectionX = (x2 - x1) / length;
    const double lineDirectionY = (y2 - y1) / length;
    const double perpendicularDirectionX = lineDirectionY;
    const double perpendicularDirectionY = -lineDirectionX;
    const double perpendicularSize = size * 0.4;

    const auto computePoint = [&](int32_t i, double side) {
      const double x =
          x1 + lineDirectionX * (actualElementSize * i - offset) + perpendicularDirectionX * perpendicularSize * side;
      const double y =
          y1 + lineDirectionY * (actualElementSize * i - offset) + perpendicularDirectionY * perpendicularSize * side;
      return std::make_pair(x, y);
    };

    // First point
    zigZagLineString = std::format("M {}", DrawingUtilities::getPointString({x1, y1}));

    // Second point
    double side = 1;
    zigZagLineString += std::format(" L {}", DrawingUtilities::getPointString(computePoint(1, side)));
    side = -side;

    // All other point segments
    for (int32_t i = 1; i < segmentsAmount; i++) {
      zigZagLineString += std::format(" L {}", DrawingUtilities::getPointString(computePoint(i + 1, side)));
      side = -side;
    }
    // Add last point
    zigZagLineString += std::format(" L {}", DrawingUtilities::getPointString({x2, y2}));
  }

  std::string result = std::format("<{} d=\"{}\" fill=\"transparent\"", tagName, zigZagLineString);
  if (stroke) {
    result += std::format(" stroke=\"{}\"", stroke.value());
  }
  if (strokeWidth) {
    result += std::format(" stroke-width=\"{}\"", StringUtilities::number(strokeWidth.value()));
  }
  result += "/>";
  return result;
}
