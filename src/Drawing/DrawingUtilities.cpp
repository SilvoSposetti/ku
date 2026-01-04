#include "DrawingUtilities.hpp"

#include "StringUtilities.hpp"

#include <format>

std::string DrawingUtilities::getPointString(const std::pair<double, double>& point) {
  return std::format("{},{}", StringUtilities::number(point.first), StringUtilities::number(point.second));
}
