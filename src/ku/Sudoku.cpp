#include "Sudoku.hpp"

#include "Setter.hpp"
#include "constraints/ConstraintFactory.hpp"
#include "drawing/DrawingOptions.hpp"
#include "drawing/DrawingUtilities.hpp"
#include "drawing/SvgGroup.hpp"
#include "drawing/SvgLine.hpp"
#include "drawing/SvgRect.hpp"
#include "drawing/SvgText.hpp"
#include "solver/ItemData.hpp"
#include "solver/Solver.hpp"

#include <algorithm>
#include <iostream>

Sudoku::Sudoku(const std::string& name,
               ConstraintType constraintTypes,
               int32_t givenDigits,
               std::optional<int32_t> seed)
    : name(name)
    , constraints(getConstraintsList(constraintTypes))
    , seed(seed)
    , board(Setter::generate(givenDigits, constraints, seed)) {}

Sudoku::Sudoku(const std::string& name,
               const std::vector<std::vector<int32_t>>& clues,
               ConstraintType constraintTypes,
               std::optional<int32_t> seed)
    : name(name)
    , constraints(getConstraintsList(constraintTypes))
    , seed(seed)
    , board(Setter::generate(transformClues(clues), constraints, seed)) {}

std::vector<std::unique_ptr<AbstractConstraint>> Sudoku::getConstraintsList(const ConstraintType constraintTypes) {
  std::vector<std::unique_ptr<AbstractConstraint>> constraintList;

  // SUDOKU_CELL constraint is always present
  constraintList.emplace_back(ConstraintFactory::makeConstraint(ConstraintType::SUDOKU_CELL));

  for (int32_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
    const uint64_t valueToCheck = static_cast<uint64_t>(1) << bitToCheck;
    if (static_cast<uint64_t>(constraintTypes) & valueToCheck) {
      const ConstraintType singleConstraint = static_cast<ConstraintType>(valueToCheck);
      if (singleConstraint != ConstraintType::SUDOKU_CELL) {
        constraintList.emplace_back(ConstraintFactory::makeConstraint(singleConstraint));
      }
    }
  }

  return constraintList;
}

bool Sudoku::isSolvable() {
  return Solver::isSolvable(constraints);
}

bool Sudoku::verify() {
  // First, check if solution satisfies the constraints
  bool solutionIsSatisfactory = true;
  const std::vector<std::vector<Sudo::Digit>> solution = board->getSolution();
  for (const auto& constraint : constraints) {
    solutionIsSatisfactory &= constraint->satisfy(solution);
  }
  if (!solutionIsSatisfactory) {
    return false;
  }

  // Then, solve the sudoku, and see if the solution is unique
  if (getGivenDigitsAmount() != Sudo::TOTAL_DIGITS) {
    const std::vector<std::vector<bool>> givenMask = board->getGivenMask();
    return Solver::isUnique(board->getField(), constraints);
  }
  return true;
}

std::vector<std::vector<Sudo::Digit>> Sudoku::getSolution() {
  return board->getSolution();
}

std::vector<std::vector<Sudo::Digit>> Sudoku::getField() {
  return board->getField();
}

std::vector<std::vector<bool>> Sudoku::getGivenMask() {
  return board->getGivenMask();
}

void Sudoku::exportToSvg(const std::filesystem::path& location) {
  DrawingOptions options(1000, 150, constraints.size());
  auto document = std::make_unique<SvgDocument>(name, options.size, options.totalHeight, options.margin);

  // Background
  document->addBackground("white");

  // Title and description
  {
    auto titleAndDescriptionsGroup =
        std::make_unique<SvgGroup>("Title And Descriptions", "black", std::nullopt, std::nullopt);
    titleAndDescriptionsGroup->add(std::make_unique<SvgText>(
        0, options.titleBaseHeight, name, options.titleFontSize, TextAnchor::Start, TextBaseline::Bottom));
    const std::string infoDescription = std::to_string(getGivenDigitsAmount()) + " (-" +
                                        std::to_string(Sudo::TOTAL_DIGITS - getGivenDigitsAmount()) + ")";
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
    const auto solution = board->getSolution();
    const auto givenMask = board->getGivenMask();
    auto givenDigitsGroup = std::make_unique<SvgGroup>("Given Digits", "black", std::nullopt, std::nullopt);
    auto otherDigitsGroup = std::make_unique<SvgGroup>("Other Digits", "black", std::nullopt, std::nullopt);
    int32_t i = 0;
    for (const auto& row : solution) {
      int32_t j = 0;
      for (const auto& digit : row) {
        const double x = (j + 0.5) * options.cellSize;
        const double y = (i + 0.5) * options.cellSize;
        const std::string digitString = std::to_string(static_cast<int32_t>(digit));
        if (givenMask[i][j]) {
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

  document->writeToFile(location);
}

void Sudoku::exportExactCoverMatrixToSvg(const std::filesystem::path& location) {
  auto document = createExactCoverDocument(
      name + "-ExactCover", DancingCellsStructure(board->getField(), constraints), constraints);
  document->writeToFile(location);
}

void Sudoku::printInfo() {
  // Name
  std::string info = name + " | ";
  // Given digits
  info += std::to_string(getGivenDigitsAmount()) + " (-" + std::to_string(Sudo::TOTAL_DIGITS - getGivenDigitsAmount()) +
          ") | ";
  // Constraints
  info += "[";
  std::string constraintsNames;
  for (const auto& constraint : constraints) {
    constraintsNames += constraint->getName() + ", ";
  }
  if (!constraintsNames.empty()) {
    constraintsNames = constraintsNames.substr(0, constraintsNames.size() - 2);
  }
  info += constraintsNames;
  info += "]";
  std::cout << info << std::endl;
}

void Sudoku::printBoard() {
  board->print();
}

int32_t Sudoku::getGivenDigitsAmount() const {
  int32_t givenDigitsAmount = 0;
  for (const auto& row : board->getField()) {
    for (const auto& digit : row) {
      if (digit != Sudo::Digit::NONE) {
        givenDigitsAmount++;
      }
    }
  }
  return givenDigitsAmount;
}

std::vector<std::vector<Sudo::Digit>> Sudoku::transformClues(const std::vector<std::vector<int32_t>>& clues) {

  if (clues.size() != Sudo::MAX_DIGIT) {
    return Sudo::emptyField();
  }
  if (std::any_of(
          clues.begin(), clues.end(), [&](const std::vector<int32_t>& row) { return row.size() != Sudo::MAX_DIGIT; })) {
    return Sudo::emptyField();
  }

  std::vector<std::vector<Sudo::Digit>> transformedClues;
  for (const auto& row : clues) {
    std::vector<Sudo::Digit> transformedRow;
    for (const auto& digit : row) {
      transformedRow.emplace_back(static_cast<Sudo::Digit>(digit));
    }
    transformedClues.emplace_back(transformedRow);
  }
  return transformedClues;
}

std::unique_ptr<SvgDocument>
Sudoku::createExactCoverDocument(const std::string& name,
                                 const DancingCellsStructure& dataStructure,
                                 const std::vector<std::unique_ptr<AbstractConstraint>>& constraints) {

  auto itemsData = std::vector<ItemData>(dataStructure.itemsCount);
  {
    int32_t counter = 0;
    for (const auto& constraint : constraints) {
      int32_t constraintCounter = 0;
      for (int32_t i = 0; i < constraint->getPrimaryItemsAmount(); i++) {
        itemsData[counter] = ItemData(constraint->getName(), true, counter, constraintCounter);
        constraintCounter++;
        counter++;
      }
      for (int32_t i = 0; i < constraint->getSecondaryItemsAmount(); i++) {
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

    std::pair<int32_t, int32_t> previousCell{-1, -1};
    int32_t counter = 0;
    for (const auto& optionData : dataStructure.optionsData) {
      const double y = cellSize * counter;
      std::pair<int32_t, int32_t> currentCell = std::make_pair(optionData.indexI, optionData.indexJ);
      if (previousCell != currentCell) {

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
      for (const auto& optionData : dataStructure.optionsData) {
        const std::string optionName = "Row " + std::to_string(optionData.indexI) + ", Column " +
                                       std::to_string(optionData.indexJ) + ", Digit " +
                                       std::to_string(static_cast<int32_t>(optionData.digit));
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
}
