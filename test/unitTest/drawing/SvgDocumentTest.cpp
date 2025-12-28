#include "drawing/SvgDocument.hpp"

#include "drawing/DrawingUtilities.hpp"
#include "drawing/SvgCircle.hpp"
#include "drawing/SvgGroup.hpp"
#include "drawing/SvgLine.hpp"
#include "drawing/SvgPolyline.hpp"
#include "drawing/SvgRect.hpp"
#include "drawing/SvgText.hpp"
#include "utilities/FileIo.hpp"
#include "utilities/TemporaryDirectory.hpp"

#include <doctest.h>
#include <numbers>

TEST_SUITE("Drawing") {
  TemporaryDirectory temporaryDirectory;
  const std::filesystem::path path = temporaryDirectory.path() / "Test" / "Document";

  TEST_CASE("Empty Document") {
    SvgDocument document("Empty", 11, 12, 5);
    const std::string expected = "<?xml version=\"1.0\"?>\n"
                                 "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" "
                                 "viewBox=\"-5 -5 21 22\" font-family=\"Open Sans\">\n"
                                 "</svg>";
    CHECK_EQ(expected, document.string());

    CHECK(document.writeToFile(path));

    auto readText = FileIo::read(path / "Empty.svg");
    CHECK(readText.has_value());
    if (readText.has_value()) {
      CHECK_EQ(expected, readText.value());
    }
  }

  TEST_CASE("Shapes") {
    SvgDocument document("Shapes", 100, 100, 20);
    // Background
    document.addBackground("white");
    // Border
    document.add(std::make_unique<SvgRect>(0, 0, 100, 100, "rgba(0,0,0,0)", "black", 1));
    // Group
    std::unique_ptr<SvgGroup> group = std::make_unique<SvgGroup>("Group", "blue", "gray", 2);
    constexpr float size = 8;
    for (int32_t i = 0; i < 5; i++) {
      // Rect
      group->add(std::make_unique<SvgRect>(
          90 - i * size - size / 2, 10 + i * size - size / 2, size, size, std::nullopt, std::nullopt, i));
      // Line
      const float x1 = std::max((i - 2) * size, 0.0f);
      const float y1 = std::max((-i + 2) * size, 0.0f);
      const float x2 = std::min(100 - (i - 2) * size, 100.0f);
      const float y2 = std::min(100 - (-i + 2) * size, 100.0f);
      group->add(std::make_unique<SvgLine>(x1, y1, x2, y2, "green", std::nullopt));
      // Circle
      group->add(std::make_unique<SvgCircle>(10 + i * size, 90 - i * size, size / 2, "red", "black", i));
    }
    document.add(std::move(group));

    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-20 -20 140 140\" "
        "font-family=\"Open Sans\">\n"
        "<rect x=\"-20\" y=\"-20\" width=\"140\" height=\"140\" fill=\"white\"/>\n"
        "<rect x=\"0\" y=\"0\" width=\"100\" height=\"100\" fill=\"rgba(0,0,0,0)\" stroke=\"black\" "
        "stroke-width=\"1\"/>\n"
        "<g id=\"Group\" fill=\"blue\" stroke=\"gray\" stroke-width=\"2\">\n"
        "<rect x=\"86\" y=\"6\" width=\"8\" height=\"8\" stroke-width=\"0\"/>\n"
        "<line x1=\"0\" y1=\"16\" x2=\"100\" y2=\"84\" stroke=\"green\"/>\n"
        "<circle cx=\"10\" cy=\"90\" r=\"4\" fill=\"red\" stroke=\"black\" stroke-width=\"0\"/>\n"
        "<rect x=\"78\" y=\"14\" width=\"8\" height=\"8\" stroke-width=\"1\"/>\n"
        "<line x1=\"0\" y1=\"8\" x2=\"100\" y2=\"92\" stroke=\"green\"/>\n"
        "<circle cx=\"18\" cy=\"82\" r=\"4\" fill=\"red\" stroke=\"black\" stroke-width=\"1\"/>\n"
        "<rect x=\"70\" y=\"22\" width=\"8\" height=\"8\" stroke-width=\"2\"/>\n"
        "<line x1=\"0\" y1=\"0\" x2=\"100\" y2=\"100\" stroke=\"green\"/>\n"
        "<circle cx=\"26\" cy=\"74\" r=\"4\" fill=\"red\" stroke=\"black\" stroke-width=\"2\"/>\n"
        "<rect x=\"62\" y=\"30\" width=\"8\" height=\"8\" stroke-width=\"3\"/>\n"
        "<line x1=\"8\" y1=\"0\" x2=\"92\" y2=\"100\" stroke=\"green\"/>\n"
        "<circle cx=\"34\" cy=\"66\" r=\"4\" fill=\"red\" stroke=\"black\" stroke-width=\"3\"/>\n"
        "<rect x=\"54\" y=\"38\" width=\"8\" height=\"8\" stroke-width=\"4\"/>\n"
        "<line x1=\"16\" y1=\"0\" x2=\"84\" y2=\"100\" stroke=\"green\"/>\n"
        "<circle cx=\"42\" cy=\"58\" r=\"4\" fill=\"red\" stroke=\"black\" stroke-width=\"4\"/>\n"
        "</g>\n"
        "</svg>";
    CHECK_EQ(expected, document.string());

    CHECK(document.writeToFile(path));

    auto readText = FileIo::read(path / "Shapes.svg");
    CHECK(readText.has_value());
    if (readText.has_value()) {
      CHECK_EQ(expected, readText.value());
    }
  }

  TEST_CASE("Text") {
    constexpr double documentSize = 200;
    constexpr double margin = 5;
    SvgDocument document("Text", documentSize, documentSize, margin);
    // Background
    document.addBackground("white");
    // Border
    document.add(std::make_unique<SvgRect>(0, 0, documentSize, documentSize, "rgba(0,0,0,0)", "black", 1));

    std::vector<std::tuple<TextAnchor, TextBaseline, std::string>> options = {
        {TextAnchor::Start, TextBaseline::Bottom, "red"},
        {TextAnchor::Start, TextBaseline::Central, "green"},
        {TextAnchor::Start, TextBaseline::Hanging, "blue"},
        {TextAnchor::Middle, TextBaseline::Bottom, "cyan"},
        {TextAnchor::Middle, TextBaseline::Central, "yellow"},
        {TextAnchor::Middle, TextBaseline::Hanging, "magenta"},
        {TextAnchor::End, TextBaseline::Bottom, "lightgray"},
        {TextAnchor::End, TextBaseline::Central, "darkgray"},
        {TextAnchor::End, TextBaseline::Hanging, "black"},
    };
    const double offset = documentSize / static_cast<double>(options.size() + 1);
    int32_t i = 0;
    const double leftCenter = documentSize / 4.0;
    const double middleCenter = leftCenter * 2.0;
    const double rightCenter = leftCenter * 3.0;
    const double fontSize = offset / 2;

    const int32_t angleCount = 3;
    const double angleIncrement = 360.0 / angleCount;
    document.add(std::make_unique<SvgLine>(leftCenter, 0, leftCenter, documentSize, "black", 0.1));
    document.add(std::make_unique<SvgLine>(middleCenter, 0, middleCenter, documentSize, "black", 0.1));
    document.add(std::make_unique<SvgLine>(rightCenter, 0, rightCenter, documentSize, "black", 0.1));
    for (const auto& [anchor, baseline, fill] : options) {
      const double centerHeight = (static_cast<double>(i) + 0.5) * offset;
      document.add(std::make_unique<SvgLine>(0, centerHeight, documentSize, centerHeight, "black", 0.1));
      document.add(std::make_unique<SvgText>(
          leftCenter, centerHeight, "AaBbCc", fontSize, anchor, baseline, fill, std::nullopt));
      document.add(
          std::make_unique<SvgText>(middleCenter, centerHeight, std::to_string(i), fontSize, anchor, baseline));
      for (int32_t j = 0; j < angleCount; j++) {
        const auto angle = j * angleIncrement;
        document.add(std::make_unique<SvgText>(rightCenter,
                                               centerHeight,
                                               DrawingUtilities::number(angle) + "deg",
                                               fontSize / 3,
                                               anchor,
                                               baseline,
                                               fill,
                                               angle));
      }
      i++;
    }

    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-5 -5 210 210\" "
        "font-family=\"Open Sans\">\n"
        "<rect x=\"-5\" y=\"-5\" width=\"210\" height=\"210\" fill=\"white\"/>\n"
        "<rect x=\"0\" y=\"0\" width=\"200\" height=\"200\" fill=\"rgba(0,0,0,0)\" stroke=\"black\" "
        "stroke-width=\"1\"/>\n"
        "<line x1=\"50\" y1=\"0\" x2=\"50\" y2=\"200\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<line x1=\"100\" y1=\"0\" x2=\"100\" y2=\"200\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<line x1=\"150\" y1=\"0\" x2=\"150\" y2=\"200\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<line x1=\"0\" y1=\"10\" x2=\"200\" y2=\"10\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"10\" font-size=\"10\" text-anchor=\"start\" alignment-baseline=\"text-bottom\" "
        "fill=\"red\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"10\" font-size=\"10\" text-anchor=\"start\" alignment-baseline=\"text-bottom\">0</text>\n"
        "<text x=\"150\" y=\"10\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"text-bottom\" "
        "fill=\"red\" transform=\"rotate(0, 150, 10)\">0deg</text>\n"
        "<text x=\"150\" y=\"10\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"text-bottom\" "
        "fill=\"red\" transform=\"rotate(120, 150, 10)\">120deg</text>\n"
        "<text x=\"150\" y=\"10\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"text-bottom\" "
        "fill=\"red\" transform=\"rotate(240, 150, 10)\">240deg</text>\n"
        "<line x1=\"0\" y1=\"30\" x2=\"200\" y2=\"30\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"30\" font-size=\"10\" text-anchor=\"start\" alignment-baseline=\"central\" "
        "fill=\"green\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"30\" font-size=\"10\" text-anchor=\"start\" alignment-baseline=\"central\">1</text>\n"
        "<text x=\"150\" y=\"30\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"central\" "
        "fill=\"green\" transform=\"rotate(0, 150, 30)\">0deg</text>\n"
        "<text x=\"150\" y=\"30\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"central\" "
        "fill=\"green\" transform=\"rotate(120, 150, 30)\">120deg</text>\n"
        "<text x=\"150\" y=\"30\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"central\" "
        "fill=\"green\" transform=\"rotate(240, 150, 30)\">240deg</text>\n"
        "<line x1=\"0\" y1=\"50\" x2=\"200\" y2=\"50\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"50\" font-size=\"10\" text-anchor=\"start\" alignment-baseline=\"hanging\" "
        "fill=\"blue\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"50\" font-size=\"10\" text-anchor=\"start\" alignment-baseline=\"hanging\">2</text>\n"
        "<text x=\"150\" y=\"50\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"hanging\" "
        "fill=\"blue\" transform=\"rotate(0, 150, 50)\">0deg</text>\n"
        "<text x=\"150\" y=\"50\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"hanging\" "
        "fill=\"blue\" transform=\"rotate(120, 150, 50)\">120deg</text>\n"
        "<text x=\"150\" y=\"50\" font-size=\"3.333\" text-anchor=\"start\" alignment-baseline=\"hanging\" "
        "fill=\"blue\" transform=\"rotate(240, 150, 50)\">240deg</text>\n"
        "<line x1=\"0\" y1=\"70\" x2=\"200\" y2=\"70\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"70\" font-size=\"10\" text-anchor=\"middle\" alignment-baseline=\"text-bottom\" "
        "fill=\"cyan\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"70\" font-size=\"10\" text-anchor=\"middle\" alignment-baseline=\"text-bottom\">3</text>\n"
        "<text x=\"150\" y=\"70\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"text-bottom\" "
        "fill=\"cyan\" transform=\"rotate(0, 150, 70)\">0deg</text>\n"
        "<text x=\"150\" y=\"70\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"text-bottom\" "
        "fill=\"cyan\" transform=\"rotate(120, 150, 70)\">120deg</text>\n"
        "<text x=\"150\" y=\"70\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"text-bottom\" "
        "fill=\"cyan\" transform=\"rotate(240, 150, 70)\">240deg</text>\n"
        "<line x1=\"0\" y1=\"90\" x2=\"200\" y2=\"90\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"90\" font-size=\"10\" text-anchor=\"middle\" alignment-baseline=\"central\" "
        "fill=\"yellow\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"90\" font-size=\"10\" text-anchor=\"middle\" alignment-baseline=\"central\">4</text>\n"
        "<text x=\"150\" y=\"90\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"central\" "
        "fill=\"yellow\" transform=\"rotate(0, 150, 90)\">0deg</text>\n"
        "<text x=\"150\" y=\"90\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"central\" "
        "fill=\"yellow\" transform=\"rotate(120, 150, 90)\">120deg</text>\n"
        "<text x=\"150\" y=\"90\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"central\" "
        "fill=\"yellow\" transform=\"rotate(240, 150, 90)\">240deg</text>\n"
        "<line x1=\"0\" y1=\"110\" x2=\"200\" y2=\"110\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"110\" font-size=\"10\" text-anchor=\"middle\" alignment-baseline=\"hanging\" "
        "fill=\"magenta\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"110\" font-size=\"10\" text-anchor=\"middle\" alignment-baseline=\"hanging\">5</text>\n"
        "<text x=\"150\" y=\"110\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"hanging\" "
        "fill=\"magenta\" transform=\"rotate(0, 150, 110)\">0deg</text>\n"
        "<text x=\"150\" y=\"110\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"hanging\" "
        "fill=\"magenta\" transform=\"rotate(120, 150, 110)\">120deg</text>\n"
        "<text x=\"150\" y=\"110\" font-size=\"3.333\" text-anchor=\"middle\" alignment-baseline=\"hanging\" "
        "fill=\"magenta\" transform=\"rotate(240, 150, 110)\">240deg</text>\n"
        "<line x1=\"0\" y1=\"130\" x2=\"200\" y2=\"130\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"130\" font-size=\"10\" text-anchor=\"end\" alignment-baseline=\"text-bottom\" "
        "fill=\"lightgray\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"130\" font-size=\"10\" text-anchor=\"end\" alignment-baseline=\"text-bottom\">6</text>\n"
        "<text x=\"150\" y=\"130\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"text-bottom\" "
        "fill=\"lightgray\" transform=\"rotate(0, 150, 130)\">0deg</text>\n"
        "<text x=\"150\" y=\"130\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"text-bottom\" "
        "fill=\"lightgray\" transform=\"rotate(120, 150, 130)\">120deg</text>\n"
        "<text x=\"150\" y=\"130\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"text-bottom\" "
        "fill=\"lightgray\" transform=\"rotate(240, 150, 130)\">240deg</text>\n"
        "<line x1=\"0\" y1=\"150\" x2=\"200\" y2=\"150\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"150\" font-size=\"10\" text-anchor=\"end\" alignment-baseline=\"central\" "
        "fill=\"darkgray\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"150\" font-size=\"10\" text-anchor=\"end\" alignment-baseline=\"central\">7</text>\n"
        "<text x=\"150\" y=\"150\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"central\" "
        "fill=\"darkgray\" transform=\"rotate(0, 150, 150)\">0deg</text>\n"
        "<text x=\"150\" y=\"150\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"central\" "
        "fill=\"darkgray\" transform=\"rotate(120, 150, 150)\">120deg</text>\n"
        "<text x=\"150\" y=\"150\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"central\" "
        "fill=\"darkgray\" transform=\"rotate(240, 150, 150)\">240deg</text>\n"
        "<line x1=\"0\" y1=\"170\" x2=\"200\" y2=\"170\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<text x=\"50\" y=\"170\" font-size=\"10\" text-anchor=\"end\" alignment-baseline=\"hanging\" "
        "fill=\"black\">AaBbCc</text>\n"
        "<text x=\"100\" y=\"170\" font-size=\"10\" text-anchor=\"end\" alignment-baseline=\"hanging\">8</text>\n"
        "<text x=\"150\" y=\"170\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"hanging\" "
        "fill=\"black\" transform=\"rotate(0, 150, 170)\">0deg</text>\n"
        "<text x=\"150\" y=\"170\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"hanging\" "
        "fill=\"black\" transform=\"rotate(120, 150, 170)\">120deg</text>\n"
        "<text x=\"150\" y=\"170\" font-size=\"3.333\" text-anchor=\"end\" alignment-baseline=\"hanging\" "
        "fill=\"black\" transform=\"rotate(240, 150, 170)\">240deg</text>\n"
        "</svg>";

    CHECK_EQ(expected, document.string());

    CHECK(document.writeToFile(path));

    auto readText = FileIo::read(path / "Text.svg");
    CHECK(readText.has_value());
    if (readText.has_value()) {
      CHECK_EQ(expected, readText.value());
    }
  }

  TEST_CASE("Polyline") {
    constexpr double size = 100;
    constexpr double margin = 10;

    SvgDocument document("Polyline", size, size, margin);
    // Background
    document.addBackground("white");
    // Border
    document.add(std::make_unique<SvgRect>(0, 0, 100, 100, "rgba(0,0,0,0)", "black", 1));

    std::vector<std::tuple<double,
                           double,
                           int32_t,
                           double,
                           double,
                           bool,
                           std::optional<std::string>,
                           std::optional<std::string>,
                           std::optional<int32_t>>>
        polylineSpecs = {
            {25, 25, 5, 8, 20, false, std::nullopt, std::nullopt, std::nullopt},
            {75, 25, 2, 5, 15, false, "rgba(0,0,0,0)", "magenta", 1},
            {25, 75, 7, 15, 17, true, "gray", "lightgray", 1},
            {75, 75, 3, 15, 7, false, "cyan", "yellow", 5},
        };
    for (const auto& [centerX, centerY, spikes, innerRadius, outerRadius, close, fill, stroke, strokeWidth] :
         polylineSpecs) {
      const double angleStep = (2.0 * std::numbers::pi) / static_cast<double>(spikes * 2);
      std::vector<std::pair<double, double>> pointsList;
      for (int32_t i = 0; i < spikes * 2; i++) {
        const double angle = i * angleStep;
        const double radius = i % 2 == 0 ? innerRadius : outerRadius;
        const double x = centerX + radius * std::cos(angle);
        const double y = centerY - radius * std::sin(angle);
        pointsList.emplace_back(std::make_pair(x, y));
      }
      document.add(std::make_unique<SvgPolyline>(pointsList, close, fill, stroke, strokeWidth));
    }

    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-10 -10 120 120\" "
        "font-family=\"Open Sans\">\n"
        "<rect x=\"-10\" y=\"-10\" width=\"120\" height=\"120\" fill=\"white\"/>\n"
        "<rect x=\"0\" y=\"0\" width=\"100\" height=\"100\" fill=\"rgba(0,0,0,0)\" stroke=\"black\" "
        "stroke-width=\"1\"/>\n"
        "<polyline points=\"33,25 41.18,13.244 27.472,17.392 18.82,5.979 18.528,20.298 5,25 18.528,29.702 18.82,44.021 "
        "27.472,32.608 41.18,36.756\"/>\n"
        "<polyline points=\"80,25 75,10 70,25 75,40\" fill=\"rgba(0,0,0,0)\" stroke=\"magenta\" stroke-width=\"1\"/>\n"
        "<polygon points=\"40,75 40.316,67.624 34.352,63.273 28.783,58.426 21.662,60.376 14.401,61.709 11.485,68.492 "
        "8,75 11.485,81.508 14.401,88.291 21.662,89.624 28.783,91.574 34.352,86.727 40.316,82.376 40,75\" "
        "fill=\"gray\" stroke=\"lightgray\" stroke-width=\"1\"/>\n"
        "<polyline points=\"90,75 78.5,68.938 67.5,62.01 68,75 67.5,87.99 78.5,81.062\" fill=\"cyan\" "
        "stroke=\"yellow\" stroke-width=\"5\"/>\n"
        "</svg>";

    CHECK_EQ(expected, document.string());

    CHECK(document.writeToFile(path));

    auto readText = FileIo::read(path / "Polyline.svg");
    CHECK(readText.has_value());
    if (readText.has_value()) {
      CHECK_EQ(expected, readText.value());
    }
  }
}
