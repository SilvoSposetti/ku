#include "StringUtilities.hpp"

#include <cmath>
#include <format>

std::string StringUtilities::number(double number) {
  if (std::trunc(number) == number) {
    return std::format("{}", static_cast<int32_t>(number));
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

std::string StringUtilities::padLeft(const std::string& input, char character, int32_t n) {
  if (input.size() < static_cast<size_t>(n)) {
    return std::string(n - input.size(), character) + input;
  }
  return input;
}
