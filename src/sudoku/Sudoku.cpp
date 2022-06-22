#include "Sudoku.h"

#include "Setter.h"
#include "SvgUtilities.h"
#include "constraints/ConstraintFactory.h"
#include "solver/Solver.h"

Sudoku::Sudoku(const std::string& name, int32_t totalDigits, ConstraintType constraintTypes, SymmetryType symmetryType)
    : name(name) {

  // Retrieve constraint list
  constraints = getConstraintsList(constraintTypes);

  // Minimum for unique solution: 17
  if (constraintTypes == (ConstraintType::SUDOKU_COLUMN | ConstraintType::SUDOKU_ROW | ConstraintType::SUDOKU_BOX)) {
    digitsAmount = clamp(totalDigits, 17, 81);
  } else {
    digitsAmount = clamp(totalDigits, 0, 81);
  }

  board = Setter::generate(digitsAmount, symmetryType, constraints);

  std::cout << name << " [" << digitsAmount << "|-" << std::to_string(81 - digitsAmount)
            << "]    MaskType: " << std::to_string(static_cast<int>(symmetryType)) << std::endl;
  board->print();
  // board->printGivenPattern();
  std::cout << std::endl;
  exportSudokuToSvg();
  exportDlxMatrixToSvg();
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

void Sudoku::exportSudokuToSvg() {
  std::string outputPath;
#ifndef OUT_DIR
#else
  outputPath = OUT_DIR;
#endif

  std::string outputFilePath = outputPath + "/" + name + ".svg";
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

void Sudoku::exportDlxMatrixToSvg() {
  std::string outputPath;
#ifndef OUT_DIR
#else
  outputPath = OUT_DIR;
#endif

  std::string outputFilePath = outputPath + "/" + name + "_DLX.svg";
  std::ofstream outfile(outputFilePath);

  std::string svgContent;

  // DLX Matrix
  std::vector<std::tuple<std::string, int32_t, int32_t>> constraintsInfo;
  for (const auto& constraint : constraints) {
    int32_t optionalRowsAmount =
        constraint->hasOptionalConstraints() ? constraint->getDlxConstraintColumnsAmount() : 0;
    constraintsInfo.emplace_back(
        std::make_tuple(constraint->getName(), constraint->getDlxConstraintColumnsAmount(), optionalRowsAmount));
  }

  svgContent +=
      SvgUtilities::dlxMatrix(Solver::getDlxMatrix(board->getField(), constraints), constraintsInfo);

  // Footer
  svgContent += SvgUtilities::getSvgFooter();

  // Stram it to file, then save and close
  outfile << svgContent;
  outfile.close();
}
