#include "DrawingUtilities.h"

#include <cmath>
#include <format>

std::string DrawingUtilities::number(double number) {
  if (trunc(number) == number) {
    return std::to_string(static_cast<int32_t>(number));
  }
  // Format with the necessary precision
  std::string formatted = std::format("{:.3f}", number);
  // Erase all trailing zeroes
  formatted.erase(formatted.find_last_not_of('0') + 1, std::string::npos);
  // Remove the dot if the number has been rounded to an integer
  if (formatted.ends_with(".")) {
    formatted.pop_back();
  }
  // Remove the "negative zero" result, as this is simply zero
  if (formatted == "-0") {
    return "0";
  }
  return formatted;
}

std::string DrawingUtilities::padLeft(const std::string& input, char character, int32_t n) {
  if (static_cast<size_t>(n) > input.size()) {
    std::string output = input;
    output.insert(0, n - input.size(), character);
    return output;
  }
  return input;
}

std::string DrawingUtilities::getPointString(const std::pair<double, double>& point) {
  return DrawingUtilities::number(point.first) + "," + DrawingUtilities::number(point.second);
}