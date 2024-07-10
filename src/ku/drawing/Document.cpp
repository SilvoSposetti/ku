#include "Document.h"

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

void Document::writeToFile(const std::filesystem::path& directory) const {
  const std::filesystem::path outputFilePath = directory / (name + ".svg");
  std::ofstream outfile(outputFilePath);
  outfile << string();
  outfile.close();
}
