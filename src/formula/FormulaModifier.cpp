#include "formula/FormulaModifier.hpp"

FormulaModifier::FormulaModifier(
    size_t num_clauses,
    size_t num_variables
)
    : current_quantifier_block(0),
      active_clauses(num_clauses, true),
      variable_status(
          num_variables+1,
          VariableStatus::ACTIVE
      )
{
}

void FormulaModifier::set_variable_status(
    VariableID var,
    VariableStatus status
    ) {
  variable_status.at(var) = status;
}
