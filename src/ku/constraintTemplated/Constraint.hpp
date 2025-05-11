#pragma once

#include "../constraints/ConstraintType.hpp"
#include "../puzzles/OptionsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"
#include "ConstraintInterface.hpp"

#include <string_view>
#include <vector>

/** Base class of all constraints.
 * Is also the base class of the Curiously Recurring Template Pattern (CRTP) and its derived classes are named
 * ConcreteConstraint.
 */
template <typename ConcreteConstraint, PuzzleIntrinsics puzzle>
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
      , primaryOptions(createOptions(ConcreteConstraint::primaryOption))
      , primaryItemsAmount(countUniqueElementsInOptions(primaryOptions))
      , secondaryOptions(createOptions(ConcreteConstraint::secondaryOption))
      , secondaryItemsAmount(countUniqueElementsInOptions(secondaryOptions)) {};

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
  virtual const std::optional<OptionsList<puzzle>>& getPrimaryOptions() const override {
    return primaryOptions;
  }
  virtual size_t getSecondaryItemsAmount() const override {
    return secondaryItemsAmount;
  }
  virtual const std::optional<OptionsList<puzzle>>& getSecondaryOptions() const override {
    return secondaryOptions;
  }

private:
  template <typename OptionCreatingFunction>
  static constexpr std::optional<OptionsList<puzzle>> createOptions(OptionCreatingFunction optionFunction) {
    auto options = OptionsList<puzzle>();
    size_t counter = 0;
    auto allOptionsDefined = true;
    for (const auto& [row, column, digit] : puzzle.allPossibilities) {
      const auto option =
          optionFunction(static_cast<uint32_t>(row), static_cast<uint32_t>(column), static_cast<uint32_t>(digit));
      if (option) {
        options[counter] = option.value();
      } else {
        allOptionsDefined = false;
      }
      counter++;
    };
    if (allOptionsDefined) {
      return options;
    }
    return std::nullopt;
  }

  static constexpr size_t countUniqueElementsInOptions(const std::optional<OptionsList<puzzle>> options) {
    if (!options) {
      return 0;
    }
    auto maxId = std::numeric_limits<uint32_t>::min();
    std::vector<uint32_t> set;
    for (const auto& option : options.value()) {
      for (const auto& element : option) {
        if (std::ranges::find(set, element) == set.end()) {
          set.push_back(element);
          maxId = std::max(maxId, element);
        }
      }
    }
    maxId = std::max(static_cast<uint32_t>(0), maxId);
    return std::max(set.size(), static_cast<size_t>(maxId));
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
  const std::optional<OptionsList<puzzle>> primaryOptions;
  const std::size_t primaryItemsAmount = 0;

  /** The secondary options
   */
  const std::optional<OptionsList<puzzle>> secondaryOptions;
  const std::size_t secondaryItemsAmount = 0;
};
