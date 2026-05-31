#include "explorer/GameBrancher.hpp"

ResidualQBF GameBrancher::branch(
    const ResidualQBF& qbf,
    VariableID variable,
    bool value
    ) {
  ResidualQBF child = qbf;
  child.trail =
    std::make_shared<AssignmentTrail>(
        qbf.trail,
        variable,
        value
        );
  return child;
}
