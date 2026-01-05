#include "SvgDocument.hpp"

#include "FileIo.hpp"
#include "StringUtilities.hpp"
#include "SvgRect.hpp"

#include <format>

SvgDocument::SvgDocument(const std::string& name, double width, double height, double margin)
    : SvgElement("svg", true)
    , name(name)
    , width(width)
    , height(height)
    , margin(margin) {}

std::string SvgDocument::string() const {
  const auto header = std::string{"<?xml version=\"1.0\"?>"};
  const auto metadata = std::string{"xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\""};
  const auto viewBox = std::format("viewBox=\"{} {} {} {}\"",
                                   StringUtilities::number(-margin),
                                   StringUtilities::number(-margin),
                                   StringUtilities::number(width + 2 * margin),
                                   StringUtilities::number(height + 2 * margin));
  const auto fontFamily = std::string{"font-family=\"Open Sans\""};
  std::string result = std::format("{}\n<{} {} {} {}>\n", header, tagName, metadata, viewBox, fontFamily);

  for (const auto& childElement : childElements) {
    result += std::format("{}\n", childElement->string());
  }

  result += std::format("</{}>", tagName);
  return result;
}

void SvgDocument::addBackground(const std::string& fill) {
  add(std::make_unique<SvgRect>(
      -margin, -margin, width + margin * 2, height + margin * 2, fill, std::nullopt, std::nullopt));
}

bool SvgDocument::writeToFile(const std::filesystem::path& directory) const {
  return FileIo::write(directory / (name + ".svg"), string());
}
