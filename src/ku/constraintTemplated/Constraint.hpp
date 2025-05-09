#pragma once

#include "../constraints/ConstraintType.hpp"
#include "../puzzles/OptionsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"
#include "ConstraintInterface.hpp"

#include <string_view>
#include <unordered_set>

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
      , primaryOptions(ConcreteConstraint::createPrimaryItems())
      , primaryItemsAmount(countUniqueElementsInOptions(primaryOptions))
      , secondaryOptions(ConcreteConstraint::createSecondaryItems())
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
  virtual const OptionsList<puzzle>& getPrimaryOptions() const override {
    return primaryOptions;
  }
  virtual size_t getSecondaryItemsAmount() const override {
    return secondaryItemsAmount;
  }
  virtual const OptionsList<puzzle>& getSecondaryOptions() const override {
    return secondaryOptions;
  }

private:
  static constexpr size_t countUniqueElementsInOptions(const OptionsList<puzzle> options) {
    std::unordered_set<int32_t> set;
    int32_t maxId = std::numeric_limits<int32_t>::min();
    for (const auto& option : options) {
      for (const auto& element : option) {
        set.insert(element);
        maxId = std::max(maxId, element);
      }
    }
    maxId = std::max(0, maxId);
    int32_t result = std::max(set.size(), static_cast<size_t>(maxId));
    if (result == 1) {
      return 0;
    }
    return result;
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
  const OptionsList<puzzle> primaryOptions;
  const std::size_t primaryItemsAmount = 0;

  /** The secondary options
   */
  const OptionsList<puzzle> secondaryOptions;
  const std::size_t secondaryItemsAmount = 0;
};
