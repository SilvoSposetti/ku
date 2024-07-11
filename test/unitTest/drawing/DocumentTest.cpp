#include "drawing/Document.h"

#include "doctest.h"
#include "drawing/Circle.h"
#include "drawing/Group.h"
#include "drawing/Line.h"
#include "drawing/Rect.h"

#include <fstream>

std::string readFromFile(const std::filesystem::path path) {
  std::ifstream inputFileStream(path.string());
  return std::string((std::istreambuf_iterator<char>(inputFileStream)), (std::istreambuf_iterator<char>()));
};

TEST_CASE("Document") {

  SUBCASE("Empty") {
    Document document("Empty", 11, 12, 5);
    const std::string expected =
        "<?xml version=\"1.0\"?>\n"
        "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"-5 -5 21 22\">\n"
        "</svg>";
    CHECK_EQ(expected, document.string());

    document.writeToFile(OUT_DIR);
    CHECK_EQ(expected, readFromFile(std::filesystem::path(OUT_DIR) / "Empty.svg"));
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

    CHECK_EQ(expected, readFromFile(std::filesystem::path(OUT_DIR) / "Shapes.svg"));
    document.writeToFile(OUT_DIR);
  }
}