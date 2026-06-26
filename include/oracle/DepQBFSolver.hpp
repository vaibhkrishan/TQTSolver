#pragma once

#include "common/Result.hpp"
#include "formula/ResidualQBF.hpp"
#include "oracle/FQBFSolver.hpp"

class DepQBFSolver : public FQBFSolver {
  public:
    virtual OracleResult is_fqbf(
        const ResidualQBF& qbf
        ) override;
    virtual ~DepQBFSolver() = default;
}; // class DepQBFSolver
