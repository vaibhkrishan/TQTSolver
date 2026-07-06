#include "oracle/FQBFSolver.hpp"

OracleResult FQBFSolver::is_fqbf(const ResidualQBF& qbf) {
  if (qbf.is_prefix_empty() or qbf.contains_empty_clause() or qbf.contains_pure_clause())
    return OracleResult::FALSE_RESULT;

  return OracleResult::UNKNOWN;
}
