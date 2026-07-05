#pragma once

#include <vector>

#include "common/Types.hpp"

enum class VariableStatus {
  ACTIVE=0,
  INACTIVE,
  UNUSED,
  ASSIGNED_TRUE,
  ASSIGNED_FALSE
};

class FormulaModifier {
  public:
    size_t current_quantifier_block;

    std::vector<bool> active_clauses;

    std::vector<VariableStatus> variable_status;

    FormulaModifier(
        size_t num_clauses,
        size_t num_variables
        );

    FormulaModifier(
        const FormulaModifier&
        ) = default;

    void set_variable_status(
        VariableID,
        VariableStatus
        );
};
