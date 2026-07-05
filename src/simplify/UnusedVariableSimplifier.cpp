#include "simplify/UnusedVariableSimplifier.hpp"

#include <cstdlib>

void
UnusedVariableSimplifier::apply(
    ResidualQBF& qbf
    ) {

  std::vector<bool> occurs(
      qbf.formula->num_variables + 1,
      false
      );

  const auto& clauses =
    qbf.formula->matrix.get_clauses();

  for (
      size_t i = 0;
      i < clauses.size();
      ++i
      ) {

    if (
        !qbf.modifier.active_clauses[i]
       ) {
      continue;
    }

    for (
        Literal lit
        : clauses[i]
        ) {

      VariableID var =
        std::abs(lit);

      if (
          qbf.modifier.variable_status[var]
          == VariableStatus::ACTIVE
         ) {

        occurs[var] = true;
      }
    }
  }

  for (
      size_t i = qbf.modifier.current_quantifier_block;
      i < qbf.formula->prefix.size();
      ++i
      ) {

    const auto& block =
      qbf.formula->prefix[i];

    for (
        VariableID var
        : block.get_variables()
        ) {

      if (
          qbf.modifier.variable_status[var]
          == VariableStatus::ACTIVE
          &&
          !occurs[var]
         ) {

        qbf.modifier.variable_status[var] =
          VariableStatus::UNUSED;
      }
    }
  }
}
