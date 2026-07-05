#pragma once

#include "common/Result.hpp"

#include "formula/ResidualQBF.hpp"

#include "oracle/SemanticOracle.hpp"

#include "cache/SemanticCache.hpp"

#include "simplify/SimplificationPipeline.hpp"
#include "simplify/Canonicalizer.hpp"

#include "heuristic/BranchingHeuristic.hpp"
#include "explorer/GameBrancher.hpp"

#include "explorer/SearchState.hpp"
#include "explorer/WorkQueue.hpp"

#include "io/CheckpointManager.hpp"
#include "io/TreeWriter.hpp"
#include "io/TreeReader.hpp"

#include <string>

class StrategyExplorer {
  public:

    StrategyExplorer(std::string, std::shared_ptr<QBFFormula>);
    ~StrategyExplorer();

    SemanticOracle* oracle;

    SemanticCache* cache;

    SimplificationPipeline* pipeline;

    Canonicalizer* canonicalizer;

    BranchingHeuristic* heuristic;

    GameBrancher* brancher;

    void explore();

    ResidualQBF root;

    const SearchState&
      state() const;
  private:

    std::string qbf_name_;
    SearchState state_;

    WorkQueue work_queue_;

    CheckpointManager
      checkpoint_manager_;

    TreeReader
      tree_reader_;

    TreeWriter
      tree_writer_;

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
