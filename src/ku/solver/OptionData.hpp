#pragma once
#include "../puzzles/Digits.hpp"

/** Holds information about an option
 */
struct OptionData {
  /** Default constructor
   */
  OptionData() = default;

  /** Constructor
   * @param indexI The row index of the cell that this option represents
   * @param indexJ The column index of the cell that this option represents
   * @param digit The digit that this option represents
   */
  OptionData(int32_t indexI, int32_t indexJ, Digit digit);

  /// The row index of the cell that this option represents
  int32_t indexI = -1;
  /// The column index of the cell that this option represents
  int32_t indexJ = -1;
  /// The digit that this option represents
  Digit digit = Digits::invalidDigit;
};