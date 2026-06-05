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
    StrategyExplorer();
    SemanticOracle* oracle;
    SemanticCache* cache;
    SimplificationPipeline* pipeline;
    Canonicalizer* canonicalizer;
    BranchingHeuristic* heuristic;
    GameBrancher* brancher;

    SolverResult recurse(
        ResidualQBF qbf
        );
}; // class StrategyExplorer
