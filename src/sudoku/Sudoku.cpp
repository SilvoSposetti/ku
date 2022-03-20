#include "Sudoku.h"

#include "Setter.h"
#include "SvgUtilities.h"
#include "constraints/ConstraintFactory.h"

#include <cstdint>

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
}

std::vector<std::unique_ptr<AbstractConstraint>> Sudoku::getConstraintsList(const ConstraintType constraintTypes) {
  std::vector<std::unique_ptr<AbstractConstraint>> constraintList;

  // SUDOKU_CELL constraint is always present
  constraintList.emplace_back(ConstraintFactory::makeConstraint(ConstraintType::SUDOKU_CELL));

  for (int8_t bitToCheck = 0; bitToCheck < 64; bitToCheck++) {
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

void Sudoku::exportToSvg() {
  std::string outputPath;
#ifndef OUT_DIR
#else
  outputPath = OUT_DIR;
#endif

  std::string outputFilePath = outputPath + "/" + name + ".svg";
  std::ofstream outfile(outputFilePath);

  std::vector<std::pair<std::string, std::string>> constraintDescriptions;
  for (const auto& constraint : constraints) {
    constraintDescriptions.emplace_back(constraint->getName(), constraint->getDescription());
  }
  // Get SVG string
  std::string svgContent;
  svgContent += SvgUtilities::getSvgHeader();

  // Sudoku name
  svgContent += SvgUtilities::createGroup("Title", SvgUtilities::sudokuTitle(name));

  // Constraint names and descriptions:
  int32_t line = 0;
  std::string descriptionGroup;
  for (const auto& constraint : constraints) {
    const std::string constraintText = constraint->getName() + " | " + constraint->getDescription();
    descriptionGroup += SvgUtilities::sudokuDescription(line, constraintText);
    line++;
  }
  svgContent +=
      SvgUtilities::createGroup("Description", descriptionGroup, SvgUtilities::getFontSize(descriptionFontSize));

  // Constraints visual elements
  for (const auto& constraint : constraints) {
    svgContent += constraint->getSvgGroup();
  }

  // Given digits
  std::string givenDigitsGroup;
  const std::vector<std::vector<Sudo>> solution = board->getSolution();
  const std::vector<std::vector<bool>> givenMask = board->getGivenMask();
  for (const auto& i : INDICES) {
    for (const auto& j : INDICES) {
      if (givenMask[i][j]) {
        givenDigitsGroup += SvgUtilities::givenDigit(i, j, solution[i][j]);
      }
    }
  }
  svgContent += SvgUtilities::createGroup("Given-Digits", givenDigitsGroup, givenDigitTextStyle);

  // TODO: Given pattern
  // TODO: DLX Matrix
  // TODO: (Temporary) Remaining digits

  svgContent += SvgUtilities::getSvgFooter();

  // Stram it to file, then save and close
  outfile << svgContent;
  outfile.close();
}
