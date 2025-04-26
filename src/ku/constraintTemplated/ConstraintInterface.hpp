#pragma once
#include "../constraints/ConstraintType.hpp"
#include "../puzzles/ItemsList.hpp"
#include "../puzzles/PuzzleIntrinsics.hpp"

#include <string>

/** Interface for all constraints.
 */
template <PuzzleIntrinsics puzzle>
struct ConstraintInterface {
  virtual ~ConstraintInterface() = default;
  virtual ConstraintType getType() const = 0;
  virtual std::string getName() const = 0;
  virtual std::string getDescription() const = 0;
  virtual size_t getPrimaryItemsAmount() const = 0;
  virtual ItemsList<puzzle> getPrimaryItems() const = 0;
  virtual size_t getSecondaryItemsAmount() const = 0;
  virtual ItemsList<puzzle> getSecondaryItems() const = 0;
};
