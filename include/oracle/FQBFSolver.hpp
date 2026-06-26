#pragma once

#include "common/Result.hpp"
#include "formula/ResidualQBF.hpp"

class FQBFSolver {
  public:
    virtual OracleResult is_fqbf(
        const ResidualQBF& qbf
        );
    virtual ~FQBFSolver() = default;
}; // class FQBFSolver
