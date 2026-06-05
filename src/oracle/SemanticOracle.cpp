#include "oracle/SemanticOracle.hpp"
#include "oracle/CNFTautChecker.hpp"
#include "oracle/DepQBFSolver.hpp"

SemanticOracle::SemanticOracle() {
  taut_checker = new CNFTautChecker;
  fqbf_solver = new DepQBFSolver;
}

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

bool SemanticOracle::equivalent(
    const ResidualQBF& left,
    const ResidualQBF& right
    ) {
  // TODO: encode the equivalence QBF
  // and run FQBF to check equivalence
  return false;
}
