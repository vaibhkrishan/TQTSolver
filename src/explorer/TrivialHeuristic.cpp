#include "explorer/TrivialHeuristic.hpp"

VariableID
TrivialHeuristic::choose_variable(
    const ResidualQBF& qbf
    ) {

  const QBFFormula& formula =
    qbf.materialize();

  for (const auto& block :
      formula.prefix) {


    const auto& vars = block.get_variables();
    for (VariableID var :
        vars) {

      return var;
    }
  }

  return -1;
}
