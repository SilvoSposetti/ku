#pragma once
#include "../Sudo.h"
#include "../constraints/AbstractConstraint.h"
#include "Node.h"
#include "ItemData.h"
#include "OptionData.h"

#include <vector>

class DataStructure {
public:
  DataStructure(const std::vector<std::vector<Sudo::Digit>>& board,
                const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  DataStructure(const std::vector<std::vector<bool>>& matrix, int32_t secondaryItemsAmount = 0);

  /* A copy of the structure, to use for a run of Algorithm X
   */
  std::vector<Node> getStructureCopy() const;

  int32_t getItemsAmount() const;

  const std::vector<ItemData>& getItemsData() const;

  int32_t getOptionsAmount() const;

  const std::vector<OptionData>& getOptionsData() const;

  bool isSolvableByAlgorithmX() const;

  void print() const;

private:
  static std::vector<Node> createStructure(const std::vector<ItemData>& items,
                                           const std::vector<OptionData>& options,
                                           const std::vector<std::vector<int32_t>>& optionsCache,
                                           int32_t nonZeroElementsAmount);

private:
  /* Data about the items
   */
  std::vector<ItemData> itemsData;

  /* The data about the options
   */
  std::vector<OptionData> optionsData;

  /* The structure, directly modified when Algorithm X runs
   */
  std::vector<Node> structure;

  bool isSolvable = false;
};