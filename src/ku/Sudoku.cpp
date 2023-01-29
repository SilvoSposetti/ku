#include "Sudoku.h"

#include "Setter.h"
#include "SvgUtilities.h"
#include "constraints/ConstraintFactory.h"
#include "solver/Solver.h"
#include "src/ku/utilities/Utilities.h"

Sudoku::Sudoku(const std::string& name, ConstraintType constraintTypes, int32_t givenDigits, SymmetryType symmetryType)
    : name(name) {

  // Retrieve constraint list
  constraints = getConstraintsList(constraintTypes);

  // Minimum for unique solution: 17
  if (constraintTypes == (ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_BOX)) {
    givenDigitsAmount = clamp(givenDigits, 17, 81);
  } else {
    givenDigitsAmount = clamp(givenDigits, 0, 81);
  }

  Timer timer;
  board = Setter::generate(givenDigitsAmount, symmetryType, constraints);
  timer.printElapsed("Sudoku generated         ");
}

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

bool Sudoku::isSolvable(){
  return Solver::isSolvable(constraints);
}

bool Sudoku::verify() {
  // First, check if solution satisfies the constraints
  bool solutionIsSatisfactory = true;
  const std::vector<std::vector<Sudo>> solution = board->getSolution();
  for (const auto& constraint : constraints) {
    solutionIsSatisfactory &= constraint->satisfy(solution);
  }
  if (!solutionIsSatisfactory) {
    return false;
  }

  // Then, solve the sudoku, and see if the solution is unique
  if (givenDigitsAmount != TOTAL_DIGITS) {
    const std::vector<std::vector<bool>> givenMask = board->getGivenMask();
    return Solver::isUnique(solution, givenMask, constraints);
  }
  return true;
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

  // Stram it to file, then save and close
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
    std::vector<bool> areColumnsPrimary(constraint->getDlxConstraintColumnsAmount(), false);
    for (size_t columnId = 0; columnId < areColumnsPrimary.size(); ++columnId) {
      areColumnsPrimary[columnId] = constraint->isColumnPrimary(columnId);
    }
    constraintsInfo.emplace_back(std::make_pair(constraint->getName(), areColumnsPrimary));
  }

  svgContent += SvgUtilities::dlxMatrix(Solver::getDlxMatrix(board->getField(), constraints), constraintsInfo);

  // Stream it to file, then save and close
  outfile << svgContent;
  outfile.close();
}
