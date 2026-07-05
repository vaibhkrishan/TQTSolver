#include "explorer/StrategyExplorer.hpp"
#include "heuristic/TrivialHeuristic.hpp"

#include <iostream>

StrategyExplorer::StrategyExplorer(std::string qbf_name, std::shared_ptr<QBFFormula> formula)
  :
    qbf_name_(qbf_name),
    checkpoint_manager_(qbf_name + ".explore"),
    tree_reader_(qbf_name + ".tqt"),
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
      cache,
      canonicalizer
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

    std::cout << "Exploring from saved state, total nodes: " << state_.nodes.size();
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
    std::cout << ", open nodes: " << open_count << std::endl;
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
        processed % 100
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

  auto canonical_info =  canonicalizer
    ->canonicalize(
        qbf
        );
  node.multiplier_exponent = canonical_info.multiplier_exponent;

  if (
      cache->contains(canonical_info.key)
     ) {

    auto cache_entry = cache->lookup(canonical_info.key);
    node.equivalent_node = cache_entry.node_id;
    set_status(
        node_id,
        NodeStatus::SEEN
        );

    return;
  }

  cache->insert(
      canonical_info.key,
      CacheEntry(node_id, canonical_info.multiplier_exponent)
      );

  if (
      oracle->is_taut(qbf)
     ) {

    auto taut_count =
      qbf
      .count_tautological_strategies();

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
