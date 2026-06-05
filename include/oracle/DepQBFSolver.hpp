#pragma once

#include "common/Result.hpp"
#include "formula/ResidualQBF.hpp"
#include "oracle/FQBFSolver.hpp"

class DepQBFSolver : public FQBFSolver {
  public:
    virtual bool is_fqbf(
        const ResidualQBF& qbf
        );
    virtual ~DepQBFSolver() = default;
}; // class DepQBFSolver
