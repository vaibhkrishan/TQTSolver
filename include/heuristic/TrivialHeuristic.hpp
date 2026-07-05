#pragma once

#include "formula/ResidualQBF.hpp"
#include "heuristic/BranchingHeuristic.hpp"

class TrivialHeuristic : public BranchingHeuristic{
  public:
    virtual VariableID choose_variable(
        const ResidualQBF& qbf
        );
    virtual ~TrivialHeuristic() = default;
}; // class BranchingHeuristic
