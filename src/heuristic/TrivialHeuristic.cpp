#include "heuristic/TrivialHeuristic.hpp"

VariableID
TrivialHeuristic::choose_variable(
    ResidualQBF& qbf
) {

  while (
      qbf.modifier.current_quantifier_block
      < qbf.formula->prefix.size()
      ) {

    const auto& block =
      qbf.formula->prefix[
          qbf.modifier.current_quantifier_block
      ];

    if (
        block.get_type()
        == QuantifierType::EXISTS
       ) {

      for (
          VariableID var
          : block.get_variables()
          ) {

        if (
            qbf.get_variable_status(var)
            == VariableStatus::ACTIVE
           ) {
          return var;
        }
      }
    }
    else {

      for (
          VariableID var
          : block.get_variables()
          ) {

        if (
            qbf.get_variable_status(var)
            == VariableStatus::ACTIVE
           ) {
          return var;
        }
      }

      for (
          VariableID var
          : block.get_variables()
          ) {

        if (
            qbf.get_variable_status(var)
            == VariableStatus::UNUSED
           ) {
          return var;
        }
      }
    }

    ++qbf.modifier.current_quantifier_block;
  }

  return 0;
}
