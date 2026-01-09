#include "StringUtilities.hpp"

#include <doctest.h>

TEST_CASE("String Utilities") {

  SUBCASE("Numbers to string") {
    CHECK_EQ(StringUtilities::number(0), "0");
    CHECK_EQ(StringUtilities::number(1), "1");
    CHECK_EQ(StringUtilities::number(56), "56");
    CHECK_EQ(StringUtilities::number(-5), "-5");
    CHECK_EQ(StringUtilities::number(-11101), "-11101");
    CHECK_EQ(StringUtilities::number(1.1), "1.1");
    CHECK_EQ(StringUtilities::number(-5.06), "-5.06");
    CHECK_EQ(StringUtilities::number(3.1456), "3.146");
    CHECK_EQ(StringUtilities::number(-99.12233), "-99.122");
    CHECK_EQ(StringUtilities::number(39.999999), "40");
    CHECK_EQ(StringUtilities::number(0.0000001), "0");
    CHECK_EQ(StringUtilities::number(-0.0000001), "0");
    CHECK_EQ(StringUtilities::number(-1.999999), "-2");
  }

  SUBCASE("Pad left") {
    CHECK_EQ("", StringUtilities::padLeft("", ' ', 0));
    CHECK_EQ("0", StringUtilities::padLeft("", '0', 1));
    CHECK_EQ("a", StringUtilities::padLeft("a", '0', 1));
    CHECK_EQ("0a", StringUtilities::padLeft("a", '0', 2));
    CHECK_EQ("00", StringUtilities::padLeft("", '0', 2));
    CHECK_EQ("0000a", StringUtilities::padLeft("a", '0', 5));
    CHECK_EQ("ABCDEF", StringUtilities::padLeft("ABCDEF", '0', 2));
  }
}
