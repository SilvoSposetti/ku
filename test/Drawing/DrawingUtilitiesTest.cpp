#include "DrawingUtilities.hpp"

#include <doctest.h>

TEST_SUITE("Drawing") {

  TEST_CASE("Utilities") {

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
