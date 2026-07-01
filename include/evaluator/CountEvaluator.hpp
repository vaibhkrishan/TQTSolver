#pragma once

#include <unordered_map>
#include <vector>

#include "common/Result.hpp"

#include "explorer/SearchState.hpp"
#include "formula/QBFFormula.hpp"

class CountEvaluator {
 public:

  CountEvaluator(
      const SearchState& state,
      const QBFFormula& formula
      );

  SolverResult evaluate();

 private:
  const SearchState& state_;

  const QBFFormula& formula_;

  std::unordered_map<
      NodeID,
      SolverResult
      > memo_;

  std::unordered_map<
      NodeID,
      std::vector<NodeID>
      > children_;

  std::unordered_map<
      NodeID,
      VariableID
      > branch_variable_;

  SolverResult evaluate_node(
      NodeID id
      );

  bool is_existential(
      VariableID variable
      ) const;
};
