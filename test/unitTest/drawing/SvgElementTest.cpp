#include "drawing/SvgElement.h"

#include "doctest.h"

TEST_CASE("SvgElement") {

  SUBCASE("Numbers to string") {
    CHECK_EQ(SvgElement::number(0), "0");
    CHECK_EQ(SvgElement::number(1), "1");
    CHECK_EQ(SvgElement::number(56), "56");
    CHECK_EQ(SvgElement::number(-5), "-5");
    CHECK_EQ(SvgElement::number(-11101), "-11101");
    CHECK_EQ(SvgElement::number(1.1), "1.1");
    CHECK_EQ(SvgElement::number(-5.06), "-5.06");
    CHECK_EQ(SvgElement::number(3.1456), "3.146");
    CHECK_EQ(SvgElement::number(-99.12233), "-99.122");
    CHECK_EQ(SvgElement::number(39.999999), "40");
    CHECK_EQ(SvgElement::number(0.0000001), "0");
    CHECK_EQ(SvgElement::number(-0.0000001), "0");
    CHECK_EQ(SvgElement::number(-1.999999), "-2");
  }

  SUBCASE("Pad left") {
    CHECK_EQ("", SvgElement::padLeft("", ' ', 0));
    CHECK_EQ("0", SvgElement::padLeft("", '0', 1));
    CHECK_EQ("a", SvgElement::padLeft("a", '0', 1));
    CHECK_EQ("0a", SvgElement::padLeft("a", '0', 2));
    CHECK_EQ("00", SvgElement::padLeft("", '0', 2));
    CHECK_EQ("0000a", SvgElement::padLeft("a", '0', 5));
    CHECK_EQ("ABCDEF", SvgElement::padLeft("ABCDEF", '0', 2));
  }

  SUBCASE("Point to string") {
    CHECK_EQ("0,0", SvgElement::getPointString(std::make_pair(0, 0)));
    CHECK_EQ("0,-1", SvgElement::getPointString(std::make_pair(0, -1)));
    CHECK_EQ("0,5.2", SvgElement::getPointString(std::make_pair(0, 5.2)));
    CHECK_EQ("5.6,-1.8", SvgElement::getPointString(std::make_pair(5.6, -1.8)));
    CHECK_EQ("-1,0", SvgElement::getPointString(std::make_pair(-1, 0)));
    CHECK_EQ("-9999.9,12", SvgElement::getPointString(std::make_pair(-9999.9, 12)));
  }
}