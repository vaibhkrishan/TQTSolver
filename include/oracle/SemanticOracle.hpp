#pragma once

#include "TautChecker.hpp"
#include "FQBFSolver.hpp"

class SemanticOracle {
  public:
    TautChecker* taut_checker;
    FQBFSolver* fqbf_solver;

    SemanticOracle();
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
}; // class SemanticOracle
