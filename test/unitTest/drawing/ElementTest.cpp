#include "drawing/Element.h"

#include "doctest.h"

TEST_CASE("Element") {

  SUBCASE("Numbers to string") {
    CHECK_EQ(Element::number(0), "0");
    CHECK_EQ(Element::number(1), "1");
    CHECK_EQ(Element::number(56), "56");
    CHECK_EQ(Element::number(-5), "-5");
    CHECK_EQ(Element::number(-11101), "-11101");
    CHECK_EQ(Element::number(1.1), "1.1");
    CHECK_EQ(Element::number(-5.06), "-5.06");
    CHECK_EQ(Element::number(3.1456), "3.146");
    CHECK_EQ(Element::number(-99.12233), "-99.122");
    CHECK_EQ(Element::number(39.999999), "40");
    CHECK_EQ(Element::number(0.0000001), "0");
    CHECK_EQ(Element::number(-0.0000001), "0");
    CHECK_EQ(Element::number(-1.999999), "-2");
  }
}