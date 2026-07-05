#include "heuristic/TrivialHeuristic.hpp"

VariableID
TrivialHeuristic::choose_variable(
    const ResidualQBF& qbf
    ) {

  const auto& block =
    qbf.formula->prefix[
    qbf.modifier.current_quantifier_block
    ];

  for (
      VariableID var
      : block.get_variables()
      ) {

    if (
        qbf.modifier.variable_status[var]
        == VariableStatus::ACTIVE
       ) {
      return var;
    }
  }

  std::abort();
}
