#pragma once

#include "../constraints/ConstraintType.hpp"
#include "../puzzles/OptionsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"
#include "ConstraintInterface.hpp"

#include <algorithm>
#include <string_view>
#include <vector>

/** Base class of all constraints.
 * Serves as the base class for the Curiously Recurring Template Pattern (CRTP). Its derived classes are all the
 * constraint and represented here under the name ConcreteConstraint.
 *
 */
template <typename ConcreteConstraint,
          PuzzleIntrinsics puzzle,
          std::size_t maxPrimaryOptionSize,
          std::size_t maxSecondaryOptionSize>
struct Constraint : public ConstraintInterface<puzzle> {

  /** Constructor
   * @param type The type
   * @param name The name
   * @param description The description
   */
  constexpr Constraint(ConstraintType type, const std::string_view& name, const std::string_view& description)
      : type(type)
      , name(name)
      , description(description)
      , primaryOptions(createOptions<maxPrimaryOptionSize>(ConcreteConstraint::primaryOption))
      , primaryItemsAmount(countUniqueElementsInOptions<maxPrimaryOptionSize>(primaryOptions))
      , secondaryOptions(createOptions<maxSecondaryOptionSize>(ConcreteConstraint::secondaryOption))
      , secondaryItemsAmount(countUniqueElementsInOptions<maxSecondaryOptionSize>(secondaryOptions)) {};

public:
  virtual ConstraintType getType() const override {
    return type;
  }
  virtual std::string getName() const override {
    return std::string(name);
  }
  virtual std::string getDescription() const override {
    return std::string(description);
  }
  virtual size_t getPrimaryItemsAmount() const override {
    return primaryItemsAmount;
  }

  virtual const std::optional<OptionsSpan<puzzle>> getPrimaryOptions() const override {
    return getOptions<maxPrimaryOptionSize>(primaryOptions);
  }

  virtual size_t getSecondaryItemsAmount() const override {
    return secondaryItemsAmount;
  }

  virtual const std::optional<OptionsSpan<puzzle>> getSecondaryOptions() const override {
    return getOptions<maxSecondaryOptionSize>(secondaryOptions);
  }

private:
  /** Creates the options (primary or secondary) according to the given optionFunction.
   * If all the options are empty, returns an empty optional
   * @tparam maxOptionSize The maximum size for each of the options in the list.
   * @param optionFunction The static constexpr function used to create a single option
   * @raturn If any of the options is defined, the list of all the options
   */
  template <std::size_t maxOptionSize>
  static constexpr std::optional<OptionsList<puzzle, maxOptionSize>> createOptions(const auto& optionFunction) {
    auto options = OptionsList<puzzle, maxOptionSize>();
    std::ranges::transform(puzzle.allPossibilities, options.begin(), [&](const Cell& cell) {
      return optionFunction(static_cast<uint32_t>(cell.rowIndex),
                            static_cast<uint32_t>(cell.columnIndex),
                            static_cast<uint32_t>(cell.digit));
    });
    if (std::ranges::any_of(options, [](const auto& option) { return !option.empty(); })) {
      return options;
    }
    return std::nullopt;
  }

  /** Counts the coverage of an option list. That is, how many unique optionIds are found.
   * @tparam maxOptionSize The maximum size for each of the options in the list.
   * @param options An optional list of options.
   * @return Either the amount of unique OptionIds found, or the largest one. Which one is bigger.
   */
  template <std::size_t maxOptionSize>
  static constexpr std::size_t
  countUniqueElementsInOptions(const std::optional<OptionsList<puzzle, maxOptionSize>>& options) {
    if (!options) {
      return 0;
    }
    std::vector<OptionId> set; // std::vector because function is constexpr
    // First, flatten the whole option list
    for (const auto& option : options.value()) {
      std::copy(option.begin(), option.end(), std::back_inserter(set));
    }
    // Sort and remove duplicates
    std::ranges::sort(set);
    set.erase(std::ranges::unique(set).begin(), set.end());
    // Return size of set or its maximum element
    const auto maxOptionId = static_cast<std::size_t>(std::ranges::max(set));
    return std::max(maxOptionId, set.size());
  }

  /** Counts the coverage of an option list. That is, how many unique optionIds are found.
   * @tparam maxOptionSize The maximum size for each of the options in the list.
   * @param options An optional list of options.
   * @return A list of spans referencing data of the individual options provided in the list
   */
  template <std::size_t maxOptionSize>
  std::optional<OptionsSpan<puzzle>>
  getOptions(const std::optional<OptionsList<puzzle, maxOptionSize>>& optionsList) const {
    if (optionsList.has_value()) {
      OptionsSpan<puzzle> result;
      std::ranges::transform(optionsList.value(), result.begin(), [&](const auto& option) { return option.asSpan(); });
      return result;
    }
    return {};
  }

public:
  /** The type of constraint
   */
  const ConstraintType type = ConstraintType::NONE;
  /** The name of the constraint
   */
  const std::string_view name;
  /** The description of the constraint
   */
  const std::string_view description;

  /** The primary options
   */
  const std::optional<OptionsList<puzzle, maxPrimaryOptionSize>> primaryOptions;
  /** The amount of items covered by the primary options
   */
  const std::size_t primaryItemsAmount = 0;

  /** The secondary options
   */
  const std::optional<OptionsList<puzzle, maxSecondaryOptionSize>> secondaryOptions;
  /** The amount of items covered by the secondary options
   */
  const std::size_t secondaryItemsAmount = 0;
};
