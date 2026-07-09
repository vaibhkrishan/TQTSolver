#pragma once

#include "TautChecker.hpp"
#include "FQBFSolver.hpp"

class SemanticOracle {
  public:
    TautChecker* taut_checker;
    FQBFSolver* fqbf_solver;

    SemanticOracle(bool use_qsole = true);
    bool equivalent(
        const ResidualQBF& left,
        const ResidualQBF& right
        );
    bool is_taut(
        const ResidualQBF& qbf
        );
    OracleResult is_fqbf(
        const ResidualQBF& qbf
        );
private:
    bool use_qsole_;
}; // class SemanticOracle
