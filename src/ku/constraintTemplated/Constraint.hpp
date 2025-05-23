#pragma once

#include "../constraints/ConstraintType.hpp"
#include "../puzzles/OptionsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"
#include "ConstraintInterface.hpp"

#include <string_view>
#include <vector>

/** Base class of all constraints.
 * Serves as the base class of the Curiously Recurring Template Pattern (CRTP) and its derived classes are named
 * ConcreteConstraint.
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
  template <std::size_t size>
  static constexpr std::optional<OptionsList<puzzle, size>> createOptions(auto optionFunction) {
    auto options = OptionsList<puzzle, size>();
    size_t counter = 0;
    auto atLeastOneOptionNonEmpty = false;
    for (const auto& [row, column, digit] : puzzle.allPossibilities) {
      options[counter] =
          optionFunction(static_cast<uint32_t>(row), static_cast<uint32_t>(column), static_cast<uint32_t>(digit));
      if (!options[counter].empty()) {
        atLeastOneOptionNonEmpty = true;
      }
      counter++;
    };
    if (atLeastOneOptionNonEmpty) {
      return options;
    }
    return std::nullopt;
  }

  template <std::size_t size>
  static constexpr std::size_t countUniqueElementsInOptions(const std::optional<OptionsList<puzzle, size>> options) {
    if (!options) {
      return 0;
    }
    auto maxId = std::numeric_limits<OptionId>::min();
    std::vector<OptionId> set;
    for (const auto& option : options.value()) {
      for (const auto& element : option) {
        if (std::ranges::find(set, element) == set.end()) {
          set.push_back(element);
          maxId = std::max(maxId, element);
        }
      }
    }
    maxId = std::max(static_cast<OptionId>(0), maxId);
    return std::max(set.size(), static_cast<std::size_t>(maxId));
  }

  template <std::size_t size>
  std::optional<OptionsSpan<puzzle>> getOptions(const std::optional<OptionsList<puzzle, size>>& optionsList) const {
    if (optionsList.has_value()) {
      OptionsSpan<puzzle> result;
      std::size_t index = 0;
      for (const auto& option : optionsList.value()) {
        result[index] = option.asSpan();
        index++;
      }
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
