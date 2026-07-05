#include "branching/GameBrancher.hpp"

ResidualQBF GameBrancher::branch(
    const ResidualQBF& qbf,
    VariableID variable,
    bool value
) {

    ResidualQBF child = qbf;
    child.modifier.set_variable_status(
        variable,
        (value) ? VariableStatus::ASSIGNED_TRUE : VariableStatus::ASSIGNED_FALSE
        );

    return child;
}
