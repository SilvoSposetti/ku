#include "SvgElement.hpp"

#include <doctest.h>
#include <format>
#include <print>

struct SampleSvgElement : SvgElement {
  SampleSvgElement(const std::string& tag, bool canHaveChildElements)
      : SvgElement(tag, canHaveChildElements) {};

  virtual std::string string() const override {
    auto childrenString = std::string{};
    for (const auto& child : childElements) {
      childrenString += child->string();
    }
    return std::format("{}, {}, [{}] ({}) ", tagName, canHaveChildElements, childElements.size(), childrenString);
  }
};

TEST_CASE("Svg Element") {
  auto elementWithChildren = SampleSvgElement("elementWithChildren", true);
  elementWithChildren.add(std::make_unique<SampleSvgElement>("child1", true));
  elementWithChildren.add(std::make_unique<SampleSvgElement>("child2", false));
  CHECK_EQ(elementWithChildren.string(),
           "elementWithChildren, true, [2] (child1, true, [0] () child2, false, [0] () ) ");
  auto elementWithoutChildren = SampleSvgElement("elementWithoutChildren", false);
  elementWithoutChildren.add(std::make_unique<SampleSvgElement>("child1", true));
  elementWithoutChildren.add(std::make_unique<SampleSvgElement>("child2", false));
  std::println("{}", elementWithoutChildren.string());
  CHECK_EQ(elementWithoutChildren.string(),
           "elementWithoutChildren, false, [0] () ");
}
