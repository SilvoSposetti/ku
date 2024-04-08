#pragma once
#include "../Sudo.h"
#include "../constraints/AbstractConstraint.h"
#include "Node.h"

#include <vector>

struct ItemData {
  ItemData() = default;

  ItemData(const std::string& name, bool isPrimary, int32_t itemId)
      : name(name)
      , isPrimary(isPrimary)
      , itemId(itemId) {}

  std::string name;
  bool isPrimary = true;
  int32_t itemId = -1;
};

struct OptionData {
  OptionData() = default;

  OptionData(int32_t indexI, int32_t indexJ, Sudo::Digit digit)
      : indexI(indexI)
      , indexJ(indexJ)
      , digit(digit) {}

  int32_t indexI = -1;
  int32_t indexJ = -1;
  Sudo::Digit digit = Sudo::Digit::NONE;
};

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