#pragma once

#include "common/Result.hpp"
#include "formula/ResidualQBF.hpp"

class FQBFSolver {
  public:
    virtual bool is_fqbf(
        const ResidualQBF& qbf
        ) = 0;
    virtual ~FQBFSolver() = default;
}; // class FQBFSolver
