#include "XccElement.h"

XccElement::XccElement(int32_t id)
    : id(id)
    , colorId(undefinedColor()) {}

XccElement::XccElement(int32_t id, int32_t colorId)
    : id(id)
    , colorId(colorId) {}

int32_t XccElement::undefinedColor() {
  return 0;
}
