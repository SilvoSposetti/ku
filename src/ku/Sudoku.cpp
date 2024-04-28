#include "Sudoku.h"

#include "Setter.h"
#include "SvgUtilities.h"
#include "constraints/ConstraintFactory.h"
#include "solver/Solver.h"

#include <algorithm>
#include <fstream>
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
  if (!std::filesystem::exists(location)) {
    std::filesystem::create_directories(location);
  }
  const std::filesystem::path outputFilePath = location / (name + ".svg");
  std::ofstream outfile(outputFilePath);

  // Get SVG string
  std::string svgContent;
  svgContent += SvgUtilities::getSvgHeader();

  // Background
  svgContent += SvgUtilities::background();

  // Title and description
  std::vector<std::string> constraintDescriptions;
  for (const auto& constraint : constraints) {
    constraintDescriptions.emplace_back(constraint->getName() + " | " + constraint->getDescription());
  }
  svgContent += SvgUtilities::titleAndDescription(name, constraintDescriptions);

  // Constraints visual elements
  for (const auto& constraint : constraints) {
    svgContent += constraint->getSvgGroup();
  }

  // Given digits
  svgContent += SvgUtilities::givenDigits(board->getSolution(), board->getGivenMask());

  // Footer
  svgContent += SvgUtilities::getSvgFooter();

  // Stream it to file, then save and close
  outfile << svgContent;
  outfile.close();
}

void Sudoku::exportExactCoverMatrixToSvg(const std::filesystem::path& location) {
  if (!std::filesystem::exists(location)) {
    std::filesystem::create_directories(location);
  }
  const std::filesystem::path outputFilePath = location / (name + "-ExactCover.svg");
  std::ofstream outfile(outputFilePath);

  std::string svgContent = createExactCoverMatrixSvgContents(DataStructure(board->getField(), constraints));

  // Stream it to file, then save and close
  outfile << svgContent;
  outfile.close();
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

std::string Sudoku::createExactCoverMatrixSvgContents(const DataStructure& dataStructure) {
  std::string svg;

  const int32_t columnsAmount = dataStructure.getItemsAmount();
  const int32_t rowsAmount = dataStructure.getOptionsAmount();

  const double boardSize = 1000; // Both in X and Y
  const double originX = 0;
  const double originY = 0;

  const double verticalCellSize = boardSize / rowsAmount;
  const double horizontalCellSize = boardSize / columnsAmount;
  const double cellSize = std::min(verticalCellSize, horizontalCellSize);
  const double margin = cellSize * 10;

  const double textSize = cellSize * 1;

  const double namesBuffer = cellSize * 20;
  // const double namesBuffer = 0;
  const double actualHeight = cellSize * rowsAmount + namesBuffer;
  const double actualWidth = cellSize * columnsAmount + namesBuffer;

  const double primaryLineWidth = cellSize / 5.0;
  const double secondaryLineWidth = cellSize / 20.0;

  const auto structure = dataStructure.getStructureCopy();
  // Header
  {
    const std::string header =
        "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"" + SvgUtilities::toString(originX - margin) + " " +
        SvgUtilities::toString(originY - margin) + " " + SvgUtilities::toString(actualWidth + 2.0 * margin) + " " +
        SvgUtilities::toString(actualHeight + 2.0 * margin) + "\" >\n";
    svg += header;
  }

  // Background
  {
    const std::string background = SvgUtilities::paperUnitsRect(
        0 - margin, 0 - margin, actualWidth + 2.0 * margin, actualHeight + 2.0 * margin, lightRectStyle);
    svg += background;
  }

  // Cells
  {
    std::string cells;

    int32_t currentOption = -1;
    for (const auto& node : structure) {
      if (node.type == NodeType::Spacer) {
        // go to next option
        currentOption++;
      }
      if (node.type == NodeType::Node) {
        // Compute coordinates of square
        const int32_t itemIndex = node.header - 1;
        cells += SvgUtilities::paperUnitsRect(cellSize * itemIndex, cellSize * currentOption, cellSize, cellSize);
      }
    }
    svg += SvgUtilities::createGroup("Cells", cells, darkRectStyle);
  }

  // Vertical Lines
  {
    std::string primaryLines;
    std::string secondaryLines;

    std::string currentName;
    int32_t currentIndex = 0;
    for (const auto& itemData : dataStructure.getItemsData()) {
      const double x = currentIndex * cellSize;
      if (currentName != itemData.constraintName) {
        // Add primary vertical line
        primaryLines += SvgUtilities::paperUnitsLine(x, 0, x, actualHeight);

        currentName = itemData.constraintName;
      } else {
        // Add secondary vertical line
        secondaryLines += SvgUtilities::paperUnitsLine(x, 0, x, actualHeight);
      }
      currentIndex++;
    }
    primaryLines += SvgUtilities::paperUnitsLine(currentIndex * cellSize, 0, currentIndex * cellSize, actualHeight);
    primaryLines += SvgUtilities::paperUnitsLine(actualWidth, 0, actualWidth, cellSize * rowsAmount);

    svg += SvgUtilities::createGroup(
        "Primary Vertical Lines", primaryLines, SvgUtilities::getNoFillStroke(primaryLineWidth));
    svg += SvgUtilities::createGroup(
        "Secondary Vertical Lines", secondaryLines, SvgUtilities::getNoFillStroke(secondaryLineWidth));
  }

  // Horizontal Lines
  {
    std::string primaryLines;
    std::string secondaryLines;
    std::pair<int32_t, int32_t> previousCell{-1, -1};
    int32_t counter = 0;
    for (const auto& optionData : dataStructure.getOptionsData()) {
      const double y = cellSize * counter;
      std::pair<int32_t, int32_t> currentCell = std::make_pair(optionData.indexI, optionData.indexJ);
      if (previousCell != currentCell) {
        primaryLines += SvgUtilities::paperUnitsLine(0, y, actualWidth, y);
        previousCell = currentCell;
      } else {
        secondaryLines += SvgUtilities::paperUnitsLine(0, y, actualWidth, y);
      }
      counter++;
    }
    primaryLines += SvgUtilities::paperUnitsLine(0, cellSize * counter, actualWidth, cellSize * counter);
    primaryLines += SvgUtilities::paperUnitsLine(0, actualHeight, cellSize * columnsAmount, actualHeight);

    svg += SvgUtilities::createGroup(
        "Primary Horizontal Lines", primaryLines, SvgUtilities::getNoFillStroke(primaryLineWidth));
    svg += SvgUtilities::createGroup(
        "Secondary Horizontal Lines", secondaryLines, SvgUtilities::getNoFillStroke(secondaryLineWidth));
  }

  // Text
  {
    // Bottom Text
    {
      std::string bottomText;
      int32_t counter = 0;
      for (const auto& itemData : dataStructure.getItemsData()) {
        std::string itemName = itemData.constraintName + " " + (itemData.isPrimary ? "P" : "S") + " " +
                               SvgUtilities::padLeft(std::to_string(itemData.itemId), '0', 4) + "->";
        double x = (static_cast<double>(counter) + 0.5) * cellSize;
        double y = cellSize * rowsAmount;
        bottomText += SvgUtilities::text(x, y, itemName, SvgUtilities::getRotatedTextStyle(x, y, textSize));
        counter++;
      }
      svg += SvgUtilities::createGroup("Text", bottomText);
    }
    // Right Text
    {
      std::string rightText;
      int32_t counter = 0;
      for (const auto& optionData : dataStructure.getOptionsData()) {
        std::string optionName = "<- Row " + std::to_string(optionData.indexI) + ", Column " +
                                 std::to_string(optionData.indexJ) + ", Digit " +
                                 std::to_string(static_cast<int32_t>(optionData.digit));
        double x = cellSize * columnsAmount;
        double y = (static_cast<double>(counter) + 0.5) * cellSize;
        rightText += SvgUtilities::text(x, y, optionName, SvgUtilities::getTextStyle(textSize));
        counter++;
      }
      svg += SvgUtilities::createGroup("Text", rightText);
    }
  }
  // Footer
  {
    const std::string footer = SvgUtilities::getSvgFooter();
    svg += footer;
  }
  return svg;
}
