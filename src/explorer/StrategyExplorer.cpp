#include "explorer/StrategyExplorer.hpp"
#include "explorer/TrivialHeuristic.hpp"

#include <iostream>

StrategyExplorer::StrategyExplorer(std::string qbf_name, std::shared_ptr<QBFFormula> formula)
  :
    qbf_name_(qbf_name),
    checkpoint_manager_(qbf_name + ".explore"),
    tree_reader_(qbf_name + ".explore"),
    tree_writer_(qbf_name + ".tqt")
{
  oracle = new SemanticOracle;
  cache = new SemanticCache;
  pipeline = new SimplificationPipeline;
  canonicalizer = new Canonicalizer;
  heuristic = new TrivialHeuristic;
  brancher = new GameBrancher;

  root.formula =
    formula;

  root.trail =
    nullptr;
}

StrategyExplorer::~StrategyExplorer() {
  delete oracle;
  delete cache;
  delete pipeline;
  delete canonicalizer;
  delete heuristic;
  delete brancher;
}

void
StrategyExplorer::explore(
    ) {

  tree_reader_.read(
      state_,
      root,
      brancher,
      cache
      );

  if (
      state_.nodes.empty()
     ) {

    std::cout << "Exploring from root." << std::endl;
    NodeID root_id =
      create_node(
          root,
          0
          );

    work_queue_.push(
        {root_id}
        );
  }
  else {

    std::cout << "Exploring from saved state";
    int open_count = 0;
    for (
        const auto& [id, node]
        : state_.nodes
        ) {

      if (
          node.status
          ==
          NodeStatus::OPEN
         ) {

        work_queue_.push(
            {id}
            );
        open_count++;
      }
    }
    std::cout << ", number of open nodes: " << open_count << std::endl;
  }

  WorkItem item;

  std::size_t processed = 0;

  while (
      work_queue_.pop(
        item
        )
      ) {

    process_node(
        item.id
        );

    ++processed;

    if (
        processed % 1000
        ==
        0
       ) {

      tree_writer_
        .capture(
            state_
            );

      tree_writer_
        .flush();

      checkpoint_manager_
        .flush();
    }
  }

  tree_writer_
    .capture(
        state_
        );

  tree_writer_
    .flush();

  checkpoint_manager_
    .flush();
}

NodeID
StrategyExplorer::create_node(
    const ResidualQBF& qbf,
    std::size_t depth
    ) {

  NodeID id =
    state_.allocate_node_id();

  SearchNode node;

  node.id = id;

  node.depth = depth;

  node.status =
    NodeStatus::OPEN;

  node.qbf = std::make_shared<ResidualQBF>(
      qbf
      );

  node.canonical_hash =  canonicalizer
    ->canonicalize(
        qbf
        );

  state_.nodes[id] =
    std::move(node);

  checkpoint_manager_
    .append_node(
        state_.nodes[id]
        );

  return id;
}

void
StrategyExplorer::add_edge(
    NodeID parent,
    NodeID child,
    VariableID variable,
    bool value
    ) {

  SearchEdge edge;

  edge.parent = parent;

  edge.child = child;

  edge.variable = variable;

  edge.value = value;

  state_.edges.push_back(
      edge
      );

  checkpoint_manager_
    .append_edge(edge);
}

void
StrategyExplorer::set_status(
    NodeID id,
    NodeStatus status
    ) {

  auto& node = state_.nodes[id];
  node.status =
    status;

  checkpoint_manager_
    .update_node(
        node
        );
}

void
StrategyExplorer::process_node(
    NodeID node_id
    ) {

  SearchNode& node =
    state_.nodes.at(
        node_id
        );

  auto& qbf =
    *node.qbf;

  pipeline->run(qbf);

  const auto& formula =
    qbf.materialize();

  std::string key = node.canonical_hash;

  if (
      cache->contains(key)
     ) {

    node.value = cache->lookup(key);
    set_status(
        node_id,
        NodeStatus::CACHE_HIT
        );

    return;
  }

  if (
      oracle->is_taut(qbf)
     ) {

    auto taut_count =
      qbf
      .count_tautological_strategies();

    cache->insert(
        key,
        taut_count
        );

    node.value = taut_count;
    set_status(
        node_id,
        NodeStatus::LEAF_TAUT
        );

    return;
  }

  auto fqbf_oracle_result = oracle->is_fqbf(qbf);
  if (
      fqbf_oracle_result == OracleResult::FALSE_RESULT
     ) {

    cache->insert(
        key,
        0
        );

    node.value = 0;
    set_status(
        node_id,
        NodeStatus::LEAF_FQBF
        );

    return;
  }

  VariableID variable =
    heuristic
    ->choose_variable(
        qbf
        );

  if (
      variable < 0
     ) {

    set_status(
        node_id,
        NodeStatus::LEAF_COMPLETE
        );

    return;
  }

  ResidualQBF left =
    brancher->branch(
        qbf,
        variable,
        false
        );

  ResidualQBF right =
    brancher->branch(
        qbf,
        variable,
        true
        );

  if (
      oracle->equivalent(
        left,
        right
        )
     ) {

    NodeID child_id =
      create_node(
          left,
          node.depth + 1
          );

    add_edge(
        node.id,
        child_id,
        variable,
        false
        );

    add_edge(
        node.id,
        child_id,
        variable,
        true
        );

    work_queue_.push(
        {child_id}
        );
  }
  else {

    NodeID left_id =
      create_node(
          left,
          node.depth + 1
          );

    NodeID right_id =
      create_node(
          right,
          node.depth + 1
          );

    add_edge(
        node.id,
        left_id,
        variable,
        false
        );

    add_edge(
        node.id,
        right_id,
        variable,
        true
        );

    work_queue_.push(
        {left_id}
        );

    work_queue_.push(
        {right_id}
        );
  }

  set_status(
      node_id,
      NodeStatus::EXPANDED
      );
}

const SearchState&
StrategyExplorer::state() const {
  return state_;
}
