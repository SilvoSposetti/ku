#include "Sudoku.h"

#include "Setter.h"
#include "constraints/ConstraintFactory.h"
#include "drawing/DrawingOptions.h"
#include "drawing/Group.h"
#include "drawing/Line.h"
#include "drawing/Rect.h"
#include "drawing/Text.h"
#include "solver/Solver.h"

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
  DrawingOptions options(1000, 100, constraints.size());
  auto document = std::make_unique<Document>(name, options.size, options.totalHeight, options.margin);

  // Background
  document->addBackground("white");

  // Title and description
  {
    auto titleAndDescriptionsGroup =
        std::make_unique<Group>("Title And Descriptions", "black", std::nullopt, std::nullopt);
    titleAndDescriptionsGroup->add(std::make_unique<Text>(0,
                                                          options.titleBaseHeight,
                                                          name,
                                                          options.titleFontSize,
                                                          TextAnchor::Start,
                                                          TextBaseline::Bottom,
                                                          std::nullopt,
                                                          std::nullopt));
    titleAndDescriptionsGroup->add(
        std::make_unique<Text>(options.size,
                               options.titleBaseHeight,
                               std::to_string(getGivenDigitsAmount()) + " (-" +
                                   std::to_string(Sudo::TOTAL_DIGITS - getGivenDigitsAmount()) + ")",
                               options.infoFontSize,
                               TextAnchor::End,
                               TextBaseline::Bottom,
                               std::nullopt,
                               std::nullopt));
    int32_t constraintCount = 0;
    for (const auto& constraint : constraints) {
      titleAndDescriptionsGroup->add(
          std::make_unique<Text>(0,
                                 options.infoBaseHeight + constraintCount * options.infoLineHeight,
                                 constraint->getName(),
                                 options.infoFontSize,
                                 TextAnchor::Start,
                                 TextBaseline::Bottom,
                                 std::nullopt,
                                 std::nullopt));
      titleAndDescriptionsGroup->add(
          std::make_unique<Text>(options.size,
                                 options.infoBaseHeight + constraintCount * options.infoLineHeight,
                                 constraint->getDescription(),
                                 options.infoFontSize,
                                 TextAnchor::End,
                                 TextBaseline::Bottom,
                                 std::nullopt,
                                 std::nullopt));
      constraintCount++;
    }
    document->add(std::move(titleAndDescriptionsGroup));
  }

  // Digits
  {
    const auto solution = board->getSolution();
    const auto givenMask = board->getGivenMask();
    auto givenDigitsGroup = std::make_unique<Group>("Given Digits", "black", std::nullopt, std::nullopt);
    auto otherDigitsGroup = std::make_unique<Group>("Other Digits", "black", std::nullopt, std::nullopt);
    int32_t i = 0;
    for (const auto& row : solution) {
      int32_t j = 0;
      for (const auto& digit : row) {
        const double x = (j + 0.5) * options.cellSize;
        const double y = (i + 0.5) * options.cellSize;
        const std::string digitString = std::to_string(static_cast<int32_t>(digit));
        if (givenMask[i][j]) {
          givenDigitsGroup->add(std::make_unique<Text>(x,
                                                       y,
                                                       digitString,
                                                       options.givenDigitsFontSize,
                                                       TextAnchor::Middle,
                                                       TextBaseline::Central,
                                                       "black",
                                                       std::nullopt));
        } else {
          otherDigitsGroup->add(std::make_unique<Text>(x,
                                                       y,
                                                       digitString,
                                                       options.nonGivenDigitsFontSize,
                                                       TextAnchor::Middle,
                                                       TextBaseline::Central,
                                                       "black",
                                                       std::nullopt));
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
  auto document = createExactCoverDocument(name + "-ExactCover", DataStructure(board->getField(), constraints));
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

std::unique_ptr<Document> Sudoku::createExactCoverDocument(const std::string& name,
                                                           const DataStructure& dataStructure) {
  const int32_t columnsCount = dataStructure.getItemsAmount();
  const int32_t rowsCount = dataStructure.getOptionsAmount();

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

  const auto structure = dataStructure.getStructureCopy();

  auto document = std::make_unique<Document>(name, width, height, margin);

  // Background
  document->add(std::make_unique<Rect>(
      -margin, -margin, width + 2.0 * margin, height + 2.0 * margin, "rgb(230,230,230)", std::nullopt, std::nullopt));

  // Cells
  {
    std::unique_ptr<Group> cellsGroup = std::make_unique<Group>("Cells", "black", std::nullopt, std::nullopt);

    int32_t currentOption = -1;
    for (const auto& node : structure) {
      if (node.type == NodeType::Spacer) {
        // go to next option
        currentOption++;
      }
      if (node.type == NodeType::Node) {
        // Compute coordinates of square
        const int32_t itemIndex = node.header - 1;
        cellsGroup->add(std::make_unique<Rect>(cellSize * itemIndex,
                                               cellSize * currentOption,
                                               cellSize,
                                               cellSize,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt));
      }
    }
    document->add(std::move(cellsGroup));
  }

  // Vertical Lines
  {
    std::unique_ptr<Group> primaryLinesGroup =
        std::make_unique<Group>("Primary Vertical Lines", std::nullopt, "black", primaryLineWidth);
    std::unique_ptr<Group> secondaryLinesGroup =
        std::make_unique<Group>("Secondary Vertical Lines", std::nullopt, "black", secondaryLineWidth);

    std::string currentName;
    int32_t currentIndex = 0;
    for (const auto& itemData : dataStructure.getItemsData()) {
      const double x = currentIndex * cellSize;
      if (currentName != itemData.constraintName) {
        // Add primary vertical line
        primaryLinesGroup->add(std::make_unique<Line>(x, 0, x, height, std::nullopt, std::nullopt));

        currentName = itemData.constraintName;
      } else {
        // Add secondary vertical line
        secondaryLinesGroup->add(std::make_unique<Line>(x, 0, x, height, std::nullopt, std::nullopt));
      }
      currentIndex++;
    }
    primaryLinesGroup->add(std::make_unique<Line>(
        currentIndex * cellSize, 0, currentIndex * cellSize, height, std::nullopt, std::nullopt));
    primaryLinesGroup->add(std::make_unique<Line>(width, 0, width, cellSize * rowsCount, std::nullopt, std::nullopt));

    document->add(std::move(primaryLinesGroup));
    document->add(std::move(secondaryLinesGroup));
  }

  // Horizontal Lines
  {
    std::unique_ptr<Group> primaryLinesGroup =
        std::make_unique<Group>("Primary Horizontal Lines", std::nullopt, "black", primaryLineWidth);
    std::unique_ptr<Group> secondaryLinesGroup =
        std::make_unique<Group>("Secondary Horizontal Lines", std::nullopt, "black", secondaryLineWidth);

    std::pair<int32_t, int32_t> previousCell{-1, -1};
    int32_t counter = 0;
    for (const auto& optionData : dataStructure.getOptionsData()) {
      const double y = cellSize * counter;
      std::pair<int32_t, int32_t> currentCell = std::make_pair(optionData.indexI, optionData.indexJ);
      if (previousCell != currentCell) {

        primaryLinesGroup->add(std::make_unique<Line>(0, y, width, y, std::nullopt, std::nullopt));
        previousCell = currentCell;
      } else {
        secondaryLinesGroup->add(std::make_unique<Line>(0, y, width, y, std::nullopt, std::nullopt));
      }
      counter++;
    }
    primaryLinesGroup->add(
        std::make_unique<Line>(0, cellSize * counter, width, cellSize * counter, std::nullopt, std::nullopt));
    primaryLinesGroup->add(
        std::make_unique<Line>(0, height, cellSize * columnsCount, height, std::nullopt, std::nullopt));

    document->add(std::move(primaryLinesGroup));
    document->add(std::move(secondaryLinesGroup));
  }

  // Text
  {
    std::unique_ptr<Group> textGroup = std::make_unique<Group>("Text", "black", std::nullopt, std::nullopt);
    // Bottom Text
    {
      std::unique_ptr<Group> bottomTextGroup =
          std::make_unique<Group>("Bottom Text", "black", std::nullopt, std::nullopt);
      int32_t counter = 0;
      for (const auto& itemData : dataStructure.getItemsData()) {
        const std::string itemName = itemData.constraintName + " " + (itemData.isPrimary ? "P" : "S") + " " +
                                     SvgElement::padLeft(std::to_string(itemData.itemId), '0', 4) + "->";
        double x = (static_cast<double>(counter) + 0.5) * cellSize;
        double y = cellSize * rowsCount;
        bottomTextGroup->add(std::make_unique<Text>(
            x, y, itemName, textSize, TextAnchor::End, TextBaseline::Central, std::nullopt, -90));
        counter++;
      }
      textGroup->add(std::move(bottomTextGroup));
    }
    // Right Text
    {
      std::unique_ptr<Group> rightTextGroup =
          std::make_unique<Group>("Right Text", "black", std::nullopt, std::nullopt);
      int32_t counter = 0;
      for (const auto& optionData : dataStructure.getOptionsData()) {
        const std::string optionName = "<- Row " + std::to_string(optionData.indexI) + ", Column " +
                                       std::to_string(optionData.indexJ) + ", Digit " +
                                       std::to_string(static_cast<int32_t>(optionData.digit));
        double x = cellSize * columnsCount;
        double y = (static_cast<double>(counter) + 0.5) * cellSize;
        rightTextGroup->add(std::make_unique<Text>(
            x, y, optionName, textSize, TextAnchor::Start, TextBaseline::Central, std::nullopt, std::nullopt));
        counter++;
      }
      textGroup->add(std::move(rightTextGroup));
    }

    document->add(std::move(textGroup));
  }

  return document;
}
