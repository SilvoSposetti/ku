#pragma once

#include "../../utilities/Utilities.h"
#include "../constraints/AbstractConstraint.h"
#include "Node.h"

class Solver {
public:
  static std::vector<std::vector<Sudo>>
  createNewBoard(const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  static bool isUnique(const std::vector<std::vector<Sudo>>& board,
                       const std::vector<std::vector<bool>>& givenMask,
                       const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  static std::vector<std::vector<int32_t>>
  getDlxMatrix(const std::vector<std::vector<Sudo>>& board,
               const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
               bool randomize = false);

private:
  static bool dlx(std::vector<std::vector<Sudo>>& board,
                  const std::vector<std::unique_ptr<AbstractConstraint>>& constraints,
                  bool checkForUniqueness,
                  bool randomize = false);

  static std::shared_ptr<Node>
  createDancingLinksMatrix(const std::vector<std::vector<int32_t>>& matrix,
                           const std::vector<std::unique_ptr<AbstractConstraint>>& constraints);

  static std::vector<std::vector<std::shared_ptr<Node>>> searchDlx(const std::shared_ptr<Node>& root,
                                                                   bool checkForUniqueness);

  static void searchDlxRecursive(const std::shared_ptr<Node>& root,
                                 int32_t depth,
                                 int32_t& solutionsLeftToSearchFor,
                                 std::vector<std::shared_ptr<Node>>& solutionHolder,
                                 std::vector<std::vector<std::shared_ptr<Node>>>& solutions);

  static void cover(std::shared_ptr<Node>& columnHeader);

  static void uncover(std::shared_ptr<Node>& columnHeader);

  static std::shared_ptr<Node> chooseSmallestColumn(const std::shared_ptr<Node>& root);

  static bool isMatrixSolvable(const std::vector<std::vector<int32_t>>& matrix);
};
