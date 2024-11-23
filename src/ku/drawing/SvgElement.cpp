#include "SvgElement.hpp"

SvgElement::SvgElement(const std::string& tagName, bool canHaveChildElements)
    : tagName(tagName)
    , canHaveChildElements(canHaveChildElements) {}

void SvgElement::add(std::unique_ptr<SvgElement> entity) {
  if (canHaveChildElements) {
    childElements.push_back(std::move(entity));
  }
}
