#include "drawing/DrawingUtilities.hpp"

#include <doctest.h>

TEST_SUITE("Drawing") {

  TEST_CASE("Utilities") {

    SUBCASE("Numbers to string") {
      CHECK_EQ(DrawingUtilities::number(0), "0");
      CHECK_EQ(DrawingUtilities::number(1), "1");
      CHECK_EQ(DrawingUtilities::number(56), "56");
      CHECK_EQ(DrawingUtilities::number(-5), "-5");
      CHECK_EQ(DrawingUtilities::number(-11101), "-11101");
      CHECK_EQ(DrawingUtilities::number(1.1), "1.1");
      CHECK_EQ(DrawingUtilities::number(-5.06), "-5.06");
      CHECK_EQ(DrawingUtilities::number(3.1456), "3.146");
      CHECK_EQ(DrawingUtilities::number(-99.12233), "-99.122");
      CHECK_EQ(DrawingUtilities::number(39.999999), "40");
      CHECK_EQ(DrawingUtilities::number(0.0000001), "0");
      CHECK_EQ(DrawingUtilities::number(-0.0000001), "0");
      CHECK_EQ(DrawingUtilities::number(-1.999999), "-2");
    }

    SUBCASE("Pad left") {
      CHECK_EQ("", DrawingUtilities::padLeft("", ' ', 0));
      CHECK_EQ("0", DrawingUtilities::padLeft("", '0', 1));
      CHECK_EQ("a", DrawingUtilities::padLeft("a", '0', 1));
      CHECK_EQ("0a", DrawingUtilities::padLeft("a", '0', 2));
      CHECK_EQ("00", DrawingUtilities::padLeft("", '0', 2));
      CHECK_EQ("0000a", DrawingUtilities::padLeft("a", '0', 5));
      CHECK_EQ("ABCDEF", DrawingUtilities::padLeft("ABCDEF", '0', 2));
    }

    SUBCASE("Point to string") {
      CHECK_EQ("0,0", DrawingUtilities::getPointString(std::make_pair(0, 0)));
      CHECK_EQ("0,-1", DrawingUtilities::getPointString(std::make_pair(0, -1)));
      CHECK_EQ("0,5.2", DrawingUtilities::getPointString(std::make_pair(0, 5.2)));
      CHECK_EQ("5.6,-1.8", DrawingUtilities::getPointString(std::make_pair(5.6, -1.8)));
      CHECK_EQ("-1,0", DrawingUtilities::getPointString(std::make_pair(-1, 0)));
      CHECK_EQ("-9999.9,12", DrawingUtilities::getPointString(std::make_pair(-9999.9, 12)));
      CHECK_EQ("12.345,0.001", DrawingUtilities::getPointString(std::make_pair(12.345, 0.001)));
    }
  }
}
