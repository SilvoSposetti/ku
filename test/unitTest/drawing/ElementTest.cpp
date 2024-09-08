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

  SUBCASE("Pad left") {
    CHECK_EQ("", Element::padLeft("", ' ', 0));
    CHECK_EQ("0", Element::padLeft("", '0', 1));
    CHECK_EQ("a", Element::padLeft("a", '0', 1));
    CHECK_EQ("0a", Element::padLeft("a", '0', 2));
    CHECK_EQ("00", Element::padLeft("", '0', 2));
    CHECK_EQ("0000a", Element::padLeft("a", '0', 5));
    CHECK_EQ("ABCDEF", Element::padLeft("ABCDEF", '0', 2));
  }
}