#pragma once

#include "formula/ResidualQBF.hpp"
#include "explorer/BranchingHeuristic.hpp"

class TrivialHeuristic : public BranchingHeuristic{
  public:
    virtual VariableID choose_variable(
        const ResidualQBF& qbf
        );
    virtual ~TrivialHeuristic() = default;
}; // class BranchingHeuristic
