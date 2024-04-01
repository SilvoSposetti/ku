#include "Sudoku.h"

#include "Setter.h"
#include "SvgUtilities.h"
#include "constraints/ConstraintFactory.h"
#include "solver/Solver.h"
#include "src/ku/Sudo.h"

#include <fstream>

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

void Sudoku::exportDlxMatrixToSvg(const std::filesystem::path& location) {
  if (!std::filesystem::exists(location)) {
    std::filesystem::create_directories(location);
  }
  const std::filesystem::path outputFilePath = location / (name + "-DLX.svg");
  std::ofstream outfile(outputFilePath);

  // DLX Matrix
  std::string svgContent;
  std::vector<std::pair<std::string, std::vector<bool>>> constraintsInfo;
  for (const auto& constraint : constraints) {

    std::vector<bool> areColumnsPrimary(constraint->getPrimaryItemsAmount(), true);
    std::vector<bool> secondaryColumns(constraint->getSecondaryItemsAmount(), false);
    areColumnsPrimary.insert(areColumnsPrimary.end(), secondaryColumns.begin(), secondaryColumns.end());
    constraintsInfo.emplace_back(std::make_pair(constraint->getName(), areColumnsPrimary));
  }

  svgContent += SvgUtilities::dlxMatrix(Solver::getExactCoverMatrix(board->getField(), constraints), constraintsInfo);

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
