#include "SvgElement.h"

#include <cmath>
#include <format>

SvgElement::SvgElement(const std::string& tagName, bool canHaveChildElements)
    : tagName(tagName)
    , canHaveChildElements(canHaveChildElements) {}

void SvgElement::add(std::unique_ptr<SvgElement> entity) {
  if (canHaveChildElements) {
    childElements.push_back(std::move(entity));
  }
}

std::string SvgElement::number(double number) {
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

std::string SvgElement::padLeft(const std::string& input, char character, int32_t n) {
  if (static_cast<size_t>(n) > input.size()) {
    std::string output = input;
    output.insert(0, n - input.size(), character);
    return output;
  }
  return input;
}

std::string SvgElement::getPointString(const std::pair<double, double>& point) {
  return SvgElement::number(point.first) + "," + SvgElement::number(point.second);
}