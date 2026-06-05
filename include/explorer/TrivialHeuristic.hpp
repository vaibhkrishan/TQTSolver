#pragma once

#include "formula/ResidualQBF.hpp"
#include "explorer/BranchingHeuristic.hpp"

class TrivialHeuristic : public BranchingHeuristic{
  public:
    virtual VariableID choose_variable(
        const ResidualQBF& qbf
        );
    virtual bool is_existential(
        const ResidualQBF& qbf,
        VariableID variable
        );
    virtual ~TrivialHeuristic() = default;
}; // class BranchingHeuristic
