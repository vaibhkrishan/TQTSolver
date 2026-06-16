#pragma once

#include "common/Result.hpp"

#include "explorer/SearchState.hpp"
#include "formula/QBFFormula.hpp"

class CountEvaluator {
  public:
    SolverResult evaluate(
        const SearchState& state,
        const QBFFormula& formula
        );
}; // class CountEvaluator
