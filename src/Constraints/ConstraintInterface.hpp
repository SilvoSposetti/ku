#pragma once
#include "ConstraintType.hpp"
#include "DrawingOptionsTemplated.hpp"
#include "SvgGroup.hpp"
#include "OptionsSpan.hpp"
#include "PuzzleIntrinsics.hpp"

#include <string>

/** Interface for all constraints.
 */
template <PuzzleIntrinsics puzzle>
struct ConstraintInterface {

  /** Virtual destructor to ensure proper destruction when inheriting */
  virtual ~ConstraintInterface() = default;

  /** Retrieves the type
   * @return The type
   */
  virtual ConstraintType getType() const = 0;

  /** Retrieves the name
   * @return The name
   */
  virtual std::string getName() const = 0;

  /** Retrieves the description
   * @return The description
   */
  virtual std::string getDescription() const = 0;

  /** Retrieves the amount of primary items
   * @return The amount of primary items
   */
  virtual size_t getPrimaryItemsAmount() const = 0;

  /** Retrieves the primary options
   * @return An optional list of primary options
   */
  virtual const std::optional<OptionsSpan<puzzle>> getPrimaryOptions() const = 0;

  /** Retrieves the amount of secondary items
   * @return The amount of secondary items
   */
  virtual size_t getSecondaryItemsAmount() const = 0;

  /** Retrieves the secondary options
   * @return An optional list of secondary options
   */
  virtual const std::optional<OptionsSpan<puzzle>> getSecondaryOptions() const = 0;

  /** Retrieves the SvgGroup of the constraint
   * @param options The drawing options
   * @return The SvgGroup
   */
  virtual std::unique_ptr<SvgGroup>
  getSvgGroup(const DrawingOptionsTemplated<puzzle.getPuzzleSpace()>& options) const = 0;
};
