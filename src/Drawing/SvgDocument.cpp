#include "SvgDocument.hpp"

#include "DrawingUtilities.hpp"
#include "FileIo.hpp"
#include "SvgRect.hpp"

SvgDocument::SvgDocument(const std::string& name, double width, double height, double margin)
    : SvgElement("svg", true)
    , name(name)
    , width(width)
    , height(height)
    , margin(margin) {}

std::string SvgDocument::string() const {
  std::string result = "<?xml version=\"1.0\"?>\n";
  result += "<" + tagName;
  result += " ";
  result += "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\"";
  result += " ";
  result += "viewBox=\"" + DrawingUtilities::number(-margin) + " " + DrawingUtilities::number(-margin) + " " +
            DrawingUtilities::number(width + 2 * margin) + " " + DrawingUtilities::number(height + 2 * margin) +
            "\" font-family=\"Open Sans\"";
  result += ">\n";

  for (const auto& childElement : childElements) {
    result += childElement->string() + "\n";
  }

  result += "</" + tagName + ">";
  return result;
}

void SvgDocument::addBackground(const std::string& fill) {
  add(std::make_unique<SvgRect>(
      -margin, -margin, width + margin * 2, height + margin * 2, fill, std::nullopt, std::nullopt));
}

bool SvgDocument::writeToFile(const std::filesystem::path& directory) const {
  return FileIo::write(directory / (name + ".svg"), string());
}
