#include "oracle/FQBFSolver.hpp"

OracleResult FQBFSolver::is_fqbf(const ResidualQBF& qbf) {
  const auto& formula = qbf.materialize();
  if (formula.prefix.empty() or formula.matrix.contains_empty_clause() or formula.contains_pure_clause())
    return OracleResult::FALSE_RESULT;

  if (formula.is_pure_existential())
    return OracleResult::TRUE_RESULT;

  return OracleResult::UNKNOWN;
}
