#include "../../include/oracle/SemanticOracle.hpp"

bool SemanticOracle::is_taut(
    const ResidualQBF& qbf
    ) {
  return taut_checker->is_taut(qbf);
}

bool SemanticOracle::is_fqbf(
    const ResidualQBF& qbf
    ) {
  return fqbf_solver->is_fqbf(qbf);
}

SolverResult SemanticOracle::solve_fqbf(
    const ResidualQBF& qbf
    ) {
  return fqbf_solver->solve(qbf);
}

bool SemanticOracle::equivalent(
    const ResidualQBF& left,
    const ResidualQBF& right
    ) {
  ResidualQBF reduced =
    equivalence_reducer->build_instance(
        left,
        right
        );
  return fqbf_solver->solve(reduced)
    == SolverResult::TRUE_RESULT;
}
