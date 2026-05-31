#pragma once

#include "TautChecker.hpp"
#include "FQBFSolver.hpp"
#include "EquivalenceReducer.hpp"

class SemanticOracle {
  public:
    TautChecker* taut_checker;
    FQBFSolver* fqbf_solver;
    EquivalenceReducer* equivalence_reducer;

    bool equivalent(
        const ResidualQBF& left,
        const ResidualQBF& right
        );
    bool is_taut(
        const ResidualQBF& qbf
        );
    bool is_fqbf(
        const ResidualQBF& qbf
        );
    SolverResult solve_fqbf(
        const ResidualQBF& qbf
        );
}; // class SemanticOracle
