#include "SvgSquigglyLine.hpp"

#include "FileIo.hpp"
#include "SvgDocument.hpp"
#include "SvgGroup.hpp"
#include "TemporaryDirectory.hpp"

#include <doctest.h>
#include <numbers>

TEST_SUITE("Drawing") {

  TEST_CASE("Squiggly Line") {

    TemporaryDirectory temporaryDirectory;
    const std::filesystem::path path = temporaryDirectory.path();

    const double size = 100;
    SvgDocument document("SquigglyLines", size, size, 10);
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
      group->add(std::make_unique<SvgSquigglyLine>(x, y, endX, endY, static_cast<double>(i + 1) * 2.0));
    }
    document.add(std::move(group));

    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-10 -10 120 120\" "
        "font-family=\"Open Sans\">\n"
        "<rect x=\"-10\" y=\"-10\" width=\"120\" height=\"120\" fill=\"white\"/>\n"
        "<g id=\"Lines\" stroke=\"black\" stroke-width=\"1\">\n"
        "<path d=\"M 56.25,50 C 57.292,49.2 57.292,49.2 58.333,50 S 59.375,50.8 60.417,50 S 61.458,49.2 62.5,50 S "
        "63.542,50.8 64.583,50 S 65.625,49.2 66.667,50 S 67.708,50.8 68.75,50 S 69.792,49.2 70.833,50 S 71.875,50.8 "
        "72.917,50 S 73.958,49.2 75,50 S 76.042,50.8 77.083,50 S 78.125,49.2 79.167,50 S 80.208,50.8 81.25,50 S "
        "82.292,49.2 83.333,50 S 84.375,50.8 85.417,50 S 86.458,49.2 87.5,50 S 88.542,50.8 89.583,50 S 90.625,49.2 "
        "91.667,50 S 92.708,50.8 93.75,50 S 94.792,49.2 95.833,50 S 96.875,50.8 97.917,50 S 98.958,49.2 100,50\" "
        "fill=\"transparent\"/>\n"
        "<path d=\"M 53.897,54.886 C 56.512,55.599 56.512,55.599 56.625,58.307 S 56.738,61.015 59.352,61.727 S "
        "61.967,62.44 62.08,65.148 S 62.193,67.856 64.808,68.568 S 67.423,69.281 67.536,71.989 S 67.649,74.697 "
        "70.263,75.41 S 72.878,76.122 72.991,78.83 S 73.104,81.538 75.719,82.251 S 78.334,82.963 78.447,85.671 S "
        "78.56,88.379 81.174,89.092\" fill=\"transparent\"/>\n"
        "<path d=\"M 48.609,56.093 C 50.254,59.674 50.254,59.674 47.218,62.187 S 44.183,64.699 45.828,68.28 S "
        "47.472,71.861 44.437,74.373 S 41.402,76.886 43.046,80.466 S 44.691,84.047 41.655,86.56 S 38.62,89.072 "
        "40.265,92.653 S 41.909,96.234 38.874,98.746\" fill=\"transparent\"/>\n"
        "<path d=\"M 44.369,52.712 C 41.816,57.493 41.816,57.493 36.485,56.508 S 31.155,55.523 28.602,60.305 S "
        "26.049,65.086 20.719,64.101 S 15.388,63.116 12.835,67.898 S 10.282,72.679 4.952,71.694\" "
        "fill=\"transparent\"/>\n"
        "<path d=\"M 44.369,47.288 C 37.706,48.519 37.706,48.519 34.515,42.543 S 31.323,36.566 24.66,37.797 S "
        "17.998,39.028 14.806,33.051 S 11.614,27.075 4.952,28.306\" fill=\"transparent\"/>\n"
        "<path d=\"M 48.609,43.907 C 42.307,37.866 42.307,37.866 45.364,29.689 S 48.421,21.512 42.119,15.471 S "
        "35.817,9.431 38.874,1.254\" fill=\"transparent\"/>\n"
        "<path d=\"M 53.897,45.114 C 54.065,35.921 54.065,35.921 62.989,33.712 S 71.914,31.503 72.082,22.31 S "
        "72.25,13.118 81.174,10.908\" fill=\"transparent\"/>\n"
        "</g>\n"
        "</svg>";
    CHECK_EQ(expected, document.string());

    CHECK(document.writeToFile(path));

    auto readText = FileIo::read(path / "SquigglyLines.svg");
    CHECK(readText.has_value());
    if (readText.has_value()) {
      CHECK_EQ(expected, readText.value());
    }
  }
}
