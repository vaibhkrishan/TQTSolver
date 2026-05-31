#pragma once

#include "../common/Result.hpp"

#include "../formula/ResidualQBF.hpp"

#include "../oracle/SemanticOracle.hpp"

#include "../cache/SemanticCache.hpp"

#include "../simplify/SimplificationPipeline.hpp"
#include "../simplify/Canonicalizer.hpp"

#include "BranchingHeuristic.hpp"
#include "GameBrancher.hpp"

class StrategyExplorer {
  public:
    SemanticOracle* oracle;
    SemanticCache* cache;
    SimplificationPipeline* pipeline;
    Canonicalizer* canonicalizer;
    BranchingHeuristic* heuristic;
    GameBrancher* brancher;

    SolverResult recurse(
        ResidualQBF qbf
        );
  private:
    SolverResult combine_existential(
        SolverResult left,
        SolverResult right
        );
    SolverResult combine_universal(
        SolverResult left,
        SolverResult right
        );
}; // class StrategyExplorer
