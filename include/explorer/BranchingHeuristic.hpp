#pragma once

#include "../formula/ResidualQBF.hpp"

class BranchingHeuristic {
  public:
    virtual VariableID choose_variable(
        const ResidualQBF& qbf
        ) = 0;
    virtual bool is_existential(
        const ResidualQBF& qbf,
        VariableID variable
        ) = 0;
    virtual ~BranchingHeuristic() = default;
}; // class BranchingHeuristic
