#pragma once

#include "common/Result.hpp"

#include "formula/ResidualQBF.hpp"

#include "oracle/SemanticOracle.hpp"

#include "cache/SemanticCache.hpp"

#include "simplify/SimplificationPipeline.hpp"
#include "simplify/Canonicalizer.hpp"

#include "explorer/BranchingHeuristic.hpp"
#include "explorer/GameBrancher.hpp"

#include "explorer/SearchState.hpp"
#include "explorer/WorkQueue.hpp"

#include "io/CheckpointManager.hpp"
#include "io/TreeWriter.hpp"

class StrategyExplorer {
public:

    StrategyExplorer();

    SemanticOracle* oracle;

    SemanticCache* cache;

    SimplificationPipeline* pipeline;

    Canonicalizer* canonicalizer;

    BranchingHeuristic* heuristic;

    GameBrancher* brancher;

    void explore(
        const ResidualQBF& root
    );

private:

    SearchState state_;

    WorkQueue work_queue_;

    CheckpointManager
        checkpoint_manager_;

    TreeWriter
        tree_writer_;

    NodeID create_root_node(
        const ResidualQBF& qbf
    );

    NodeID create_node(
        const ResidualQBF& qbf,
        std::size_t depth
    );

    void add_edge(
        NodeID parent,
        NodeID child,
        VariableID variable,
        bool value
    );

    void set_status(
        NodeID id,
        NodeStatus status
    );

    void process_node(
        NodeID id
    );
}; // class StrategyExplorer
