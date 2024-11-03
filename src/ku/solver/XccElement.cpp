#include "XccElement.h"

XccElement XccElement::makePrimary(int32_t id) {
  return XccElement(id, true, undefinedColor());
}

XccElement XccElement::makeSecondary(int32_t id) {
  return XccElement(id, false, undefinedColor());
}

XccElement XccElement::makeSecondary(int32_t id, int32_t colorId) {
  return XccElement(id, false, colorId);
}

int32_t XccElement::undefinedColor() {
  return 0;
}

XccElement::XccElement(int32_t id, bool isPrimary, int32_t colorId)
    : id(id)
    , isPrimary(isPrimary)
    , colorId(colorId) {}