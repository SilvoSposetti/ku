#include "Document.h"

#include "Rect.h"

#include <fstream>

Document::Document(const std::string& name, double width, double height, double margin)
    : Element("svg", true)
    , name(name)
    , width(width)
    , height(height)
    , margin(margin) {}

std::string Document::string() const {
  std::string result = "<?xml version=\"1.0\"?>\n";
  result += "<" + tagName;
  result += " ";
  result += "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\"";
  result += " ";
  result += "viewBox=\"" + number(-margin) + " " + number(-margin) + " " + number(width + 2 * margin) + " " +
            number(height + 2 * margin) + "\"";
  result += ">\n";

  for (const auto& childElement : childElements) {
    result += childElement->string() + "\n";
  }

  result += "</" + tagName + ">";
  return result;
}

void Document::addBackground(const std::string& fill) {
  add(std::make_unique<Rect>(
      -margin, -margin, width + margin * 2, height + margin * 2, fill, std::nullopt, std::nullopt));
}

void Document::writeToFile(const std::filesystem::path& directory) const {
  if (!std::filesystem::exists(directory)) {
    std::filesystem::create_directories(directory);
  }
  const std::filesystem::path outputFilePath = directory / (name + ".svg");
  std::ofstream outfile(outputFilePath);
  outfile << string();
  outfile.close();
}
