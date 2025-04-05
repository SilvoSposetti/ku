#include "Sudoku.hpp"

#include "Setter.hpp"
#include "constraints/ConstraintFactory.hpp"
#include "drawing/DataStructureDrawing.hpp"
#include "drawing/DrawingOptions.hpp"
#include "drawing/SvgGroup.hpp"
#include "drawing/SvgText.hpp"
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
  auto document = DataStructureDrawing::create(
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
