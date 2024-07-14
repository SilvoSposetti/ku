#include "drawing/Document.h"

#include "doctest.h"
#include "drawing/Circle.h"
#include "drawing/Group.h"
#include "drawing/Line.h"
#include "drawing/Rect.h"
#include "drawing/Text.h"
#include "utilities/TemporaryDirectory.h"

#include <fstream>

std::string readFromFile(const std::filesystem::path path) {
  if (!std::filesystem::exists(path)) {
    return "Path " + path.string() + " doesn't exist";
  }
  std::ifstream inputFileStream(path.string());
  return std::string((std::istreambuf_iterator<char>(inputFileStream)), (std::istreambuf_iterator<char>()));
};

TEST_CASE("Document") {
  TemporaryDirectory temporaryDirectory;
  const std::filesystem::path path = temporaryDirectory.path() / "Test" / "Document";

  SUBCASE("Empty") {
    Document document("Empty", 11, 12, 5);
    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-5 -5 21 22\">\n"
        "</svg>";
    CHECK_EQ(expected, document.string());

    document.writeToFile(path);
    CHECK_EQ(expected, readFromFile(path / "Empty.svg"));
  }

  SUBCASE("Shapes") {
    Document document("Shapes", 100, 100, 20);
    // Background
    document.add(std::make_unique<Rect>(-20, -20, 140, 140, "white", std::nullopt, std::nullopt));
    // Border
    document.add(std::make_unique<Rect>(0, 0, 100, 100, "rgba(0,0,0,0)", "black", 1));
    // Group
    std::unique_ptr<Group> group = std::make_unique<Group>("Group", "blue", "grey", 2);
    constexpr float size = 8;
    for (int32_t i = 0; i < 5; i++) {
      // Rect
      group->add(std::make_unique<Rect>(
          90 - i * size - size / 2, 10 + i * size - size / 2, size, size, std::nullopt, std::nullopt, i));
      // Line
      const float x1 = std::max((i - 2) * size, 0.0f);
      const float y1 = std::max((-i + 2) * size, 0.0f);
      const float x2 = std::min(100 - (i - 2) * size, 100.0f);
      const float y2 = std::min(100 - (-i + 2) * size, 100.0f);
      group->add(std::make_unique<Line>(x1, y1, x2, y2, "green", std::nullopt));
      // Circle
      group->add(std::make_unique<Circle>(10 + i * size, 90 - i * size, size / 2, "red", "black", i));
    }
    document.add(std::move(group));

    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-20 -20 140 140\">\n"
        "<rect x=\"-20\" y=\"-20\" width=\"140\" height=\"140\" fill=\"white\"/>\n"
        "<rect x=\"0\" y=\"0\" width=\"100\" height=\"100\" fill=\"rgba(0,0,0,0)\" stroke=\"black\" "
        "stroke-width=\"1\"/>\n"
        "<g id=\"Group\" fill=\"blue\" stroke=\"grey\" stroke-width=\"2\">\n"
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

    document.writeToFile(path);
    CHECK_EQ(expected, readFromFile(path / "Shapes.svg"));
  }

  SUBCASE("Text") {
    constexpr double documentSize = 200;
    constexpr double margin = 5;
    Document document("Text", documentSize, documentSize, margin);
    // Background
    document.add(std::make_unique<Rect>(
        -margin, -margin, documentSize + margin * 2, documentSize + margin * 2, "white", std::nullopt, std::nullopt));
    // Border
    document.add(std::make_unique<Rect>(0, 0, documentSize, documentSize, "rgba(0,0,0,0)", "black", 1));

    std::vector<std::tuple<TextAnchor, TextBaseline, std::string>> options = {
        {TextAnchor::Start, TextBaseline::Bottom, "red"},
        {TextAnchor::Start, TextBaseline::Central, "green"},
        {TextAnchor::Start, TextBaseline::Hanging, "blue"},
        {TextAnchor::Middle, TextBaseline::Bottom, "cyan"},
        {TextAnchor::Middle, TextBaseline::Central, "yellow"},
        {TextAnchor::Middle, TextBaseline::Hanging, "magenta"},
        {TextAnchor::End, TextBaseline::Bottom, "lightgrey"},
        {TextAnchor::End, TextBaseline::Central, "darkgrey"},
        {TextAnchor::End, TextBaseline::Hanging, "black"},
    };
    const double offset = documentSize / static_cast<double>(options.size());
    int32_t i = 0;
    const double leftCenter = documentSize / 3.0;
    const double rightCenter = leftCenter * 2.0;
    const double fontSize = offset / 2;
    document.add(std::make_unique<Line>(leftCenter, 0, leftCenter, documentSize, "black", 0.1));
    document.add(std::make_unique<Line>(rightCenter, 0, rightCenter, documentSize, "black", 0.1));
    for (const auto& [anchor, baseline, fill] : options) {
      const double centerHeight = (static_cast<double>(i) + 0.5) * offset;
      document.add(std::make_unique<Line>(0, centerHeight, documentSize, centerHeight, "black", 0.1));
      document.add(std::make_unique<Circle>(leftCenter, centerHeight, 1, "red", std::nullopt, std::nullopt));
      document.add(std::make_unique<Circle>(rightCenter, centerHeight, 1, "red", std::nullopt, std::nullopt));
      document.add(std::make_unique<Text>(leftCenter, centerHeight, "AaBbCc", fontSize, anchor, baseline, fill));
      document.add(std::make_unique<Text>(
          rightCenter, centerHeight, std::to_string(i), fontSize, anchor, baseline, std::nullopt));
      i++;
    }

    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-5 -5 210 210\">\n"
        "<rect x=\"-5\" y=\"-5\" width=\"210\" height=\"210\" fill=\"white\"/>\n"
        "<rect x=\"0\" y=\"0\" width=\"200\" height=\"200\" fill=\"rgba(0,0,0,0)\" stroke=\"black\" "
        "stroke-width=\"1\"/>\n"
        "<line x1=\"66.667\" y1=\"0\" x2=\"66.667\" y2=\"200\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<line x1=\"133.333\" y1=\"0\" x2=\"133.333\" y2=\"200\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<line x1=\"0\" y1=\"11.111\" x2=\"200\" y2=\"11.111\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"11.111\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"11.111\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"11.111\" font-size=\"11.111\" text-anchor=\"start\" alignment-baseline=\"text-bottom\" "
        "fill=\"red\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"11.111\" font-size=\"11.111\" text-anchor=\"start\" "
        "alignment-baseline=\"text-bottom\">0</text>\n"
        "<line x1=\"0\" y1=\"33.333\" x2=\"200\" y2=\"33.333\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"33.333\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"33.333\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"33.333\" font-size=\"11.111\" text-anchor=\"start\" alignment-baseline=\"central\" "
        "fill=\"green\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"33.333\" font-size=\"11.111\" text-anchor=\"start\" "
        "alignment-baseline=\"central\">1</text>\n"
        "<line x1=\"0\" y1=\"55.556\" x2=\"200\" y2=\"55.556\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"55.556\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"55.556\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"55.556\" font-size=\"11.111\" text-anchor=\"start\" alignment-baseline=\"hanging\" "
        "fill=\"blue\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"55.556\" font-size=\"11.111\" text-anchor=\"start\" "
        "alignment-baseline=\"hanging\">2</text>\n"
        "<line x1=\"0\" y1=\"77.778\" x2=\"200\" y2=\"77.778\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"77.778\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"77.778\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"77.778\" font-size=\"11.111\" text-anchor=\"middle\" "
        "alignment-baseline=\"text-bottom\" fill=\"cyan\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"77.778\" font-size=\"11.111\" text-anchor=\"middle\" "
        "alignment-baseline=\"text-bottom\">3</text>\n"
        "<line x1=\"0\" y1=\"100\" x2=\"200\" y2=\"100\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"100\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"100\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"100\" font-size=\"11.111\" text-anchor=\"middle\" alignment-baseline=\"central\" "
        "fill=\"yellow\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"100\" font-size=\"11.111\" text-anchor=\"middle\" "
        "alignment-baseline=\"central\">4</text>\n"
        "<line x1=\"0\" y1=\"122.222\" x2=\"200\" y2=\"122.222\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"122.222\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"122.222\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"122.222\" font-size=\"11.111\" text-anchor=\"middle\" alignment-baseline=\"hanging\" "
        "fill=\"magenta\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"122.222\" font-size=\"11.111\" text-anchor=\"middle\" "
        "alignment-baseline=\"hanging\">5</text>\n"
        "<line x1=\"0\" y1=\"144.444\" x2=\"200\" y2=\"144.444\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"144.444\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"144.444\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"144.444\" font-size=\"11.111\" text-anchor=\"end\" alignment-baseline=\"text-bottom\" "
        "fill=\"lightgrey\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"144.444\" font-size=\"11.111\" text-anchor=\"end\" "
        "alignment-baseline=\"text-bottom\">6</text>\n"
        "<line x1=\"0\" y1=\"166.667\" x2=\"200\" y2=\"166.667\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"166.667\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"166.667\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"166.667\" font-size=\"11.111\" text-anchor=\"end\" alignment-baseline=\"central\" "
        "fill=\"darkgrey\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"166.667\" font-size=\"11.111\" text-anchor=\"end\" "
        "alignment-baseline=\"central\">7</text>\n"
        "<line x1=\"0\" y1=\"188.889\" x2=\"200\" y2=\"188.889\" stroke=\"black\" stroke-width=\"0.1\"/>\n"
        "<circle cx=\"66.667\" cy=\"188.889\" r=\"1\" fill=\"red\"/>\n"
        "<circle cx=\"133.333\" cy=\"188.889\" r=\"1\" fill=\"red\"/>\n"
        "<text x=\"66.667\" y=\"188.889\" font-size=\"11.111\" text-anchor=\"end\" alignment-baseline=\"hanging\" "
        "fill=\"black\">AaBbCc</text>\n"
        "<text x=\"133.333\" y=\"188.889\" font-size=\"11.111\" text-anchor=\"end\" "
        "alignment-baseline=\"hanging\">8</text>\n"
        "</svg>";

    CHECK_EQ(expected, document.string());

    document.writeToFile(path);
    CHECK_EQ(expected, readFromFile(std::filesystem::path(path) / "Text.svg"));
  }
}