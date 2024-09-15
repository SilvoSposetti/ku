#include "drawing/SvgZigZagLine.h"

#include "doctest.h"
#include "drawing/SvgDocument.h"
#include "drawing/SvgGroup.h"
#include "utilities/FileIo.h"
#include "utilities/TemporaryDirectory.h"

#include <numbers>

TEST_SUITE("Drawing") {

  TEST_CASE("ZigZag Line") {
    TemporaryDirectory temporaryDirectory;
    const std::filesystem::path path = temporaryDirectory.path();

    const double size = 100;
    SvgDocument document("ZigZagLines", size, size, 10);
    document.addBackground("white");

    const int32_t count = 7;
    auto group = std::make_unique<SvgGroup>("Lines", std::nullopt, "black", 1);

    const double centerX = size / 2.0;
    const double centerY = size / 2.0;
    const double centerOffset = (size / 2.0) / 8.0;
    const double length = centerOffset * 7.0;
    const double angleOffset = (2.0 * std::numbers::pi) / static_cast<double>(count);
    for (int32_t i = 0; i < count; i++) {
      const double angle = angleOffset * static_cast<double>(i);
      const double x = centerX + centerOffset * std::cos(angle);
      const double y = centerY + centerOffset * std::sin(angle);
      const double endX = x + length * std::cos(angle);
      const double endY = y + length * std::sin(angle);
      group->add(std::make_unique<SvgZigZagLine>(x, y, endX, endY, static_cast<double>(i + 1) * 2.0));
    }
    document.add(std::move(group));

    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-10 -10 120 120\" "
        "font-family=\"Open Sans\">\n"
        "<rect x=\"-10\" y=\"-10\" width=\"120\" height=\"120\" fill=\"white\"/>\n"
        "<g id=\"Lines\" stroke=\"black\" stroke-width=\"1\">\n"
        "<path d=\"M 56.25,50 L 57.292,49.2 L 59.375,50.8 L 61.458,49.2 L 63.542,50.8 L 65.625,49.2 L 67.708,50.8 L "
        "69.792,49.2 L 71.875,50.8 L 73.958,49.2 L 76.042,50.8 L 78.125,49.2 L 80.208,50.8 L 82.292,49.2 L 84.375,50.8 "
        "L "
        "86.458,49.2 L 88.542,50.8 L 90.625,49.2 L 92.708,50.8 L 94.792,49.2 L 96.875,50.8 L 98.958,49.2 L 100,50\" "
        "fill=\"transparent\"/>\n"
        "<path d=\"M 53.897,54.886 L 56.512,55.599 L 56.738,61.015 L 61.967,62.44 L 62.193,67.856 L 67.423,69.281 L "
        "67.649,74.697 L 72.878,76.122 L 73.104,81.538 L 78.334,82.963 L 78.56,88.379 L 81.174,89.092\" "
        "fill=\"transparent\"/>\n"
        "<path d=\"M 48.609,56.093 L 50.254,59.674 L 44.183,64.699 L 47.472,71.861 L 41.402,76.886 L 44.691,84.047 L "
        "38.62,89.072 L 41.909,96.234 L 38.874,98.746\" fill=\"transparent\"/>\n"
        "<path d=\"M 44.369,52.712 L 41.816,57.493 L 31.155,55.523 L 26.049,65.086 L 15.388,63.116 L 10.282,72.679 L "
        "4.952,71.694\" fill=\"transparent\"/>\n"
        "<path d=\"M 44.369,47.288 L 37.706,48.519 L 31.323,36.566 L 17.998,39.028 L 11.614,27.075 L 4.952,28.306\" "
        "fill=\"transparent\"/>\n"
        "<path d=\"M 48.609,43.907 L 42.307,37.866 L 48.421,21.512 L 35.817,9.431 L 38.874,1.254\" "
        "fill=\"transparent\"/>\n"
        "<path d=\"M 53.897,45.114 L 54.065,35.921 L 71.914,31.503 L 72.25,13.118 L 81.174,10.908\" "
        "fill=\"transparent\"/>\n"
        "</g>\n"
        "</svg>";
    CHECK_EQ(expected, document.string());

    CHECK(document.writeToFile(path));

    auto readText = FileIo::read(path / "ZigZagLines.svg");
    CHECK(readText.has_value());
    if (readText.has_value()) {
      CHECK_EQ(expected, readText.value());
    }
  }
}