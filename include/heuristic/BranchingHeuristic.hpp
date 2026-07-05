#pragma once

#include "formula/ResidualQBF.hpp"

class BranchingHeuristic {
  public:
    virtual VariableID choose_variable(
        ResidualQBF& qbf
        ) = 0;
    virtual ~BranchingHeuristic() = default;
}; // class BranchingHeuristic
