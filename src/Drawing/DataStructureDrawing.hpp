#pragma once
#include "ConstraintInterface.hpp"
#include "DancingCellsStructure.hpp"
#include "DrawingUtilities.hpp"
#include "ItemData.hpp"
#include "PuzzleIntrinsics.hpp"
#include "PuzzleSpace.hpp"
#include "SvgDocument.hpp"
#include "SvgGroup.hpp"
#include "SvgLine.hpp"
#include "SvgRect.hpp"
#include "SvgText.hpp"

namespace DataStructureDrawing {

/** Creates an svg document graphically showing the exact cover matrix of the provided data structure
 * @param name The name of the document that is created
 * @param dataStructure The data structure constructed with the current constraints and field
 * @param constraints The data structure constructed with the current constraints and field
 * @return The svg document
 */
template <PuzzleSpace puzzleSpace>
std::unique_ptr<SvgDocument>
create(const std::string& name,
       const DancingCellsStructure& dataStructure,
       const std::vector<std::unique_ptr<ConstraintInterface<PuzzleIntrinsics<puzzleSpace>{}>>>& constraints,
       const std::vector<Cell>& possibilities) {

  auto itemsData = std::vector<ItemData>(dataStructure.itemsCount);
  {
    int32_t counter = 0;
    for (const auto& constraint : constraints) {
      int32_t constraintCounter = 0;
      for (size_t i = 0; i < constraint->getPrimaryItemsAmount(); i++) {
        itemsData[counter] = ItemData(constraint->getName(), true, counter, constraintCounter);
        constraintCounter++;
        counter++;
      }
      for (size_t i = 0; i < constraint->getSecondaryItemsAmount(); i++) {
        itemsData[counter] = ItemData(constraint->getName(), false, counter, constraintCounter);
        constraintCounter++;
        counter++;
      }
    }
  }

  const int32_t columnsCount = dataStructure.itemsCount;
  const int32_t rowsCount = dataStructure.optionsCount;

  const double paperSize = 1000; // Both in X and Y

  const double verticalCellSize = paperSize / rowsCount;
  const double horizontalCellSize = paperSize / columnsCount;
  const double cellSize = std::min(verticalCellSize, horizontalCellSize);
  const double margin = cellSize * 1;
  const double textSize = cellSize * 1;

  const double textLength = cellSize * 20;
  const double width = cellSize * columnsCount + textLength;
  const double height = cellSize * rowsCount + textLength;

  const double primaryLineWidth = cellSize / 5.0;
  const double secondaryLineWidth = cellSize / 20.0;

  // Create copy
  auto structure = dataStructure;

  auto document = std::make_unique<SvgDocument>(name, width, height, margin);

  // Background
  document->add(std::make_unique<SvgRect>(
      -margin, -margin, width + 2.0 * margin, height + 2.0 * margin, "rgb(230,230,230)", std::nullopt, std::nullopt));

  // Cells
  {
    std::unique_ptr<SvgGroup> cellsGroup = std::make_unique<SvgGroup>("Cells", "black", std::nullopt, std::nullopt);

    int32_t currentOption = -1;
    for (const auto& node : structure.NODE) {
      if (node.item <= 0) {
        // go to next option
        currentOption++;
        continue;
      }
      // Compute coordinates of square
      const int32_t itemIndex = structure.position(node.item);
      cellsGroup->add(std::make_unique<SvgRect>(cellSize * itemIndex, cellSize * currentOption, cellSize, cellSize));
    }
    document->add(std::move(cellsGroup));
  }

  // Vertical Lines
  {
    std::unique_ptr<SvgGroup> primaryLinesGroup =
        std::make_unique<SvgGroup>("Primary Vertical Lines", std::nullopt, "black", primaryLineWidth);
    std::unique_ptr<SvgGroup> secondaryLinesGroup =
        std::make_unique<SvgGroup>("Secondary Vertical Lines", std::nullopt, "black", secondaryLineWidth);

    std::string currentName;
    int32_t currentIndex = 0;
    for (const auto& itemData : itemsData) {
      const double x = currentIndex * cellSize;
      if (currentName != itemData.constraintName) {
        // Add primary vertical line
        primaryLinesGroup->add(std::make_unique<SvgLine>(x, 0, x, height));

        currentName = itemData.constraintName;
      } else {
        // Add secondary vertical line
        secondaryLinesGroup->add(std::make_unique<SvgLine>(x, 0, x, height));
      }
      currentIndex++;
    }
    primaryLinesGroup->add(std::make_unique<SvgLine>(currentIndex * cellSize, 0, currentIndex * cellSize, height));
    primaryLinesGroup->add(std::make_unique<SvgLine>(width, 0, width, cellSize * rowsCount));

    document->add(std::move(primaryLinesGroup));
    document->add(std::move(secondaryLinesGroup));
  }

  // Horizontal Lines
  {
    std::unique_ptr<SvgGroup> primaryLinesGroup =
        std::make_unique<SvgGroup>("Primary Horizontal Lines", std::nullopt, "black", primaryLineWidth);
    std::unique_ptr<SvgGroup> secondaryLinesGroup =
        std::make_unique<SvgGroup>("Secondary Horizontal Lines", std::nullopt, "black", secondaryLineWidth);

    auto previousCell = std::optional<Cell>();
    int32_t counter = 0;
    for (const auto& currentCell : possibilities) {
      const double y = cellSize * counter;
      if (!previousCell || !previousCell.value().isAtSameSpot(currentCell)) {
        primaryLinesGroup->add(std::make_unique<SvgLine>(0, y, width, y));
        previousCell = currentCell;
      } else {
        secondaryLinesGroup->add(std::make_unique<SvgLine>(0, y, width, y));
      }
      counter++;
    }

    primaryLinesGroup->add(std::make_unique<SvgLine>(0, cellSize * counter, width, cellSize * counter));
    primaryLinesGroup->add(std::make_unique<SvgLine>(0, height, cellSize * columnsCount, height));

    document->add(std::move(primaryLinesGroup));
    document->add(std::move(secondaryLinesGroup));
  }

  // Text
  {
    std::unique_ptr<SvgGroup> textGroup = std::make_unique<SvgGroup>("Text", "black", std::nullopt, std::nullopt);
    // Bottom Text
    {
      std::unique_ptr<SvgGroup> bottomTextGroup =
          std::make_unique<SvgGroup>("Bottom Text", "black", std::nullopt, std::nullopt);
      int32_t counter = 0;
      for (const auto& itemData : itemsData) {
        const std::string itemName = itemData.constraintName + " " +
                                     DrawingUtilities::padLeft(std::to_string(itemData.constraintItemId), '0', 4) +
                                     " " + (itemData.isPrimary ? "P" : "S") + " " +
                                     DrawingUtilities::padLeft(std::to_string(itemData.overallItemId), '0', 4);
        double x = (static_cast<double>(counter) + 0.5) * cellSize;
        double y = cellSize * rowsCount;
        bottomTextGroup->add(std::make_unique<SvgText>(
            x, y, itemName, textSize, TextAnchor::End, TextBaseline::Central, std::nullopt, -90));
        counter++;
      }
      textGroup->add(std::move(bottomTextGroup));
    }
    // Right Text
    {
      std::unique_ptr<SvgGroup> rightTextGroup =
          std::make_unique<SvgGroup>("Right Text", "black", std::nullopt, std::nullopt);
      int32_t counter = 0;

      for (const auto& [row, column, digit] : possibilities) {
        const std::string optionName =
            "Row " + std::to_string(row) + ", Column " + std::to_string(column) + ", Digit " + std::to_string(digit);
        double x = cellSize * columnsCount;
        double y = (static_cast<double>(counter) + 0.5) * cellSize;
        rightTextGroup->add(
            std::make_unique<SvgText>(x, y, optionName, textSize, TextAnchor::Start, TextBaseline::Central));
        counter++;
      }
      textGroup->add(std::move(rightTextGroup));
    }

    document->add(std::move(textGroup));
  }

  return document;
};

} // namespace DataStructureDrawing
