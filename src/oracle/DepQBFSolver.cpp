#include "oracle/DepQBFSolver.hpp"

bool DepQBFSolver::is_fqbf(const ResidualQBF& qbf) {
  // TODO: use QBF solver to implement this
  const auto& formula = qbf.materialize();
  return formula.prefix.empty();
}
