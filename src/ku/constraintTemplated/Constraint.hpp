#pragma once

#include "../constraints/ConstraintType.hpp"
#include "../puzzles/ItemsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"
#include "ConstraintInterface.hpp"

#include <string_view>
#include <unordered_set>

/** Base class for all constraints.
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
      , primaryItems(ConcreteConstraint::createPrimaryItems())
      , secondaryItems(ConcreteConstraint::createSecondaryItems()) {};

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
    return countUniqueElementsInOptions(primaryItems);
  }
  virtual ItemsList<puzzle> getPrimaryItems() const override {
    return primaryItems;
  }
  virtual size_t getSecondaryItemsAmount() const override {
    return countUniqueElementsInOptions(secondaryItems);
  }
  virtual ItemsList<puzzle> getSecondaryItems() const override {
    return secondaryItems;
  }

private:
  static size_t countUniqueElementsInOptions(const ItemsList<puzzle> options) {
    std::unordered_set<int32_t> set;
    int32_t maxId = std::numeric_limits<int32_t>::min();
    for (const auto& option : options) {
      for (const auto& element : option) {
        set.insert(element);
        maxId = std::max(maxId, element);
      }
    }
    maxId = std::max(0, maxId);
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

  /** The primary items
   */
  const ItemsList<puzzle> primaryItems;

  /** The secondary items
   */
  const ItemsList<puzzle> secondaryItems;
};
