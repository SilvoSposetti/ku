#pragma once
#include "../constraints/AbstractConstraint.hpp"
#include "../puzzles/Digits.hpp"
#include "DrawingOptions.hpp"
#include "SvgDocument.hpp"
#include "SvgGroup.hpp"
#include "SvgText.hpp"
namespace PuzzleDrawing {

template <uint8_t rowsCount, uint8_t columnsCount>
std::unique_ptr<SvgDocument> create(const std::string& name,
                                    const DrawingOptions& options,
                                    const std::array<std::array<Digit, columnsCount>, rowsCount>& grid,
                                    const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {
  auto document = std::make_unique<SvgDocument>(name, options.size, options.totalHeight, options.margin);

  // Background
  document->addBackground("white");

  // Title and description
  {

    size_t givenDigitsCount = 0;
    for (const auto& row : grid) {
      givenDigitsCount += std::ranges::count_if(row, [&](const auto& element) { return Digits::isValid(element); });
    }

    auto titleAndDescriptionsGroup =
        std::make_unique<SvgGroup>("Title And Descriptions", "black", std::nullopt, std::nullopt);
    titleAndDescriptionsGroup->add(std::make_unique<SvgText>(
        0, options.titleBaseHeight, name, options.titleFontSize, TextAnchor::Start, TextBaseline::Bottom));
    const std::string infoDescription =
        std::to_string(givenDigitsCount) + " (-" + std::to_string(Sudo::TOTAL_DIGITS - givenDigitsCount) + ")";
    titleAndDescriptionsGroup->add(std::make_unique<SvgText>(options.size,
                                                             options.titleBaseHeight,
                                                             infoDescription,
                                                             options.infoFontSize,
                                                             TextAnchor::End,
                                                             TextBaseline::Bottom));
    int32_t constraintCount = 0;
    for (const auto& constraint : constraints) {
      const double infoHeight = options.infoBaseHeight + constraintCount * options.infoLineHeight;
      const std::string constraintName = constraint->getName();
      titleAndDescriptionsGroup->add(std::make_unique<SvgText>(
          0, infoHeight, constraintName, options.infoFontSize, TextAnchor::Start, TextBaseline::Bottom));
      titleAndDescriptionsGroup->add(std::make_unique<SvgText>(options.size,
                                                               infoHeight,
                                                               constraint->getDescription(),
                                                               options.infoFontSize,
                                                               TextAnchor::End,
                                                               TextBaseline::Bottom));
      constraintCount++;
    }
    document->add(std::move(titleAndDescriptionsGroup));
  }

  // Digits
  {
    auto givenDigitsGroup = std::make_unique<SvgGroup>("Given Digits", "black", std::nullopt, std::nullopt);
    auto otherDigitsGroup = std::make_unique<SvgGroup>("Other Digits", "black", std::nullopt, std::nullopt);
    int32_t i = 0;
    for (const auto& row : grid) {
      int32_t j = 0;
      for (const auto& digit : row) {
        const double x = (j + 0.5) * options.cellSize;
        const double y = (i + 0.5) * options.cellSize;
        const std::string digitString = std::to_string(digit);
        if (Digits::isValid(digit)) {
          givenDigitsGroup->add(std::make_unique<SvgText>(
              x, y, digitString, options.givenDigitsFontSize, TextAnchor::Middle, TextBaseline::Central));
        } else {
          otherDigitsGroup->add(std::make_unique<SvgText>(
              x, y, digitString, options.nonGivenDigitsFontSize, TextAnchor::Middle, TextBaseline::Central));
        }
        j++;
      }
      i++;
    }
    document->add(std::move(givenDigitsGroup));
    document->add(std::move(otherDigitsGroup));
  }

  for (const auto& constraint : constraints) {
    document->add(constraint->getSvgGroup(options));
  }
  return document;
};
} // namespace PuzzleDrawing