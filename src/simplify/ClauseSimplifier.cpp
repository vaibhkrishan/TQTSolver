#include "simplify/ClauseSimplifier.hpp"

#include <cstdlib>

void
ClauseSimplifier::apply(
    ResidualQBF& qbf
) {

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

    bool satisfied = false;

    for (
        Literal lit
        : clauses[i]
        ) {

      VariableID var =
        std::abs(lit);

      switch (
          qbf.modifier.variable_status[var]
          ) {

        case VariableStatus::ASSIGNED_TRUE:

          if (lit > 0) {
            satisfied = true;
          }

          break;

        case VariableStatus::ASSIGNED_FALSE:

          if (lit < 0) {
            satisfied = true;
          }

          break;

        default:
          break;
      }

      if (satisfied) {
        break;
      }
    }

    if (satisfied) {
      qbf.modifier.active_clauses[i] = false;
    }
  }
}
