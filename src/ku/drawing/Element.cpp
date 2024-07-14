#include "Element.h"

#include <cmath>
#include <format>

Element::Element(const std::string& tagName, bool canHaveChildElements)
    : tagName(tagName)
    , canHaveChildElements(canHaveChildElements) {}

void Element::add(std::unique_ptr<Element> entity) {
  if (canHaveChildElements) {
    childElements.push_back(std::move(entity));
  }
}

std::string Element::number(double number) {
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