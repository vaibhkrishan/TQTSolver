#include "io/TreeReader.hpp"

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#include "explorer/NodeStatus.hpp"

TreeReader::TreeReader(
    const std::string& filename
    )
  : filename_(filename)
{
}

struct PendingNode {

  SearchNode node;

  bool seen_depth = false;

  bool seen_status = false;

  bool seen_value = false;

  bool seen_equivalent = false;

  bool seen_multiplier = false;
};

void parse_tree_file(std::ifstream& in,
    std::unordered_map<NodeID, PendingNode>& pending_nodes,
    std::vector<SearchEdge>& pending_edges) {
  std::string line;

  NodeID current_id = -1;

  while (
      std::getline(
        in,
        line
        )
      ) {

    if (
        line.empty()
       ) {
      continue;
    }

    std::stringstream ss(
        line
        );

    std::string token;

    ss >> token;

    if (
        token == "NODE"
       ) {

      NodeID id;

      ss >> id;

      PendingNode pending;

      pending.node.id = id;

      pending_nodes[id] =
        std::move(
            pending
            );

      current_id = id;

      continue;
    }

    if (
        token == "EDGE"
       ) {

      SearchEdge edge;

      ss
        >> edge.parent
        >> edge.child
        >> edge.variable
        >> edge.value;

      pending_edges
        .push_back(edge);

      continue;
    }

    if (
        current_id < 0
       ) {
      continue;
    }

    auto it =
      pending_nodes.find(
          current_id
          );

    if (
        it ==
        pending_nodes.end()
       ) {
      continue;
    }

    PendingNode& node =
      it->second;

    if (
        token == "DEPTH"
       ) {

      ss >>
        node.node.depth;

      node.seen_depth =
        true;

      continue;
    }

    if (
        token == "VALUE"
       ) {

      ss >>
        node.node.value;

      node.seen_value =
        true;

      continue;
    }

    if (
        token == "MULTIPLIER_EXPONENT"
       ) {

      ss >>
        node.node.multiplier_exponent;

      node.seen_multiplier =
        true;

      continue;
    }

    if (
        token == "EQUIVALENT_NODE"
       ) {

      ss >>
        node.node.equivalent_node;

      node.seen_equivalent =
        true;

      continue;
    }

    if (
        token == "STATUS"
       ) {

      std::string status;

      ss >> status;

      node.node.status =
        common::parse_status(
            status
            );

      node.seen_status =
        true;

      continue;
    }
  }

}

void process_pending_nodes(
    const std::unordered_map<NodeID, PendingNode>& pending_nodes,
    std::unordered_set<NodeID>& complete_nodes) {
  for (
      const auto& [id, pending]
      : pending_nodes
      ) {

    if (
        pending.seen_depth
        &&
        pending.seen_status
        &&
        pending.seen_value
        &&
        pending.seen_equivalent
        &&
        pending.seen_multiplier
       ) {

      complete_nodes
        .insert(id);
    }
  }

  if (
      !complete_nodes.contains(
        root_id
        )
     ) {
    return;
  }

}

void
compute_reachable_nodes(
    const std::unordered_set<NodeID>& complete_nodes,
    const std::vector<SearchEdge>& pending_edges,
    std::unordered_map<NodeID, std::vector<SearchEdge>>& outgoing,
    std::unordered_set<NodeID>& reachable) {
  for (
      const auto& edge
      : pending_edges
      ) {

    if (
        complete_nodes.contains(
          edge.parent
          )
        &&
        complete_nodes.contains(
          edge.child
          )
       ) {

      outgoing[
        edge.parent
      ].push_back(
          edge
          );
    }
  }

  std::queue<NodeID> q;

  q.push(root_id);

  reachable.insert(root_id);

  while (
      !q.empty()
      ) {

    NodeID current =
      q.front();

    q.pop();

    for (
        const auto& edge
        : outgoing[current]
        ) {

      if (
          reachable.insert(
            edge.child
            ).second
         ) {

        q.push(
            edge.child
            );
      }
    }
  }
}

void
TreeReader::read(
    SearchState& state,
    ResidualQBF& root,
    GameBrancher* brancher,
    SemanticCache* cache,
    Canonicalizer* canonicalizer
    ) const {
  std::ifstream in(filename_);

  if (!in) {
    return;
  }

  std::unordered_map<
    NodeID,
    PendingNode
      > pending_nodes;

  std::vector<SearchEdge>
    pending_edges;

  parse_tree_file(in, pending_nodes, pending_edges);

  std::unordered_set<NodeID>
    complete_nodes;

  process_pending_nodes(pending_nodes, complete_nodes);

  std::unordered_set<NodeID>
    reachable;

  std::unordered_map<
    NodeID,
    std::vector<SearchEdge>
      > outgoing;

  compute_reachable_nodes(complete_nodes,
      pending_edges,
      outgoing,
      reachable);

  SearchState recovered;

  NodeID max_id = root_id;

  for (
      NodeID id
      : reachable
      ) {

    recovered.nodes[id] =
      pending_nodes.at(id)
      .node;

    max_id =
      std::max(
          max_id,
          id
          );
  }

  for (
      const auto& edge
      : pending_edges
      ) {

    if (
        reachable.contains(
          edge.parent
          )
        &&
        reachable.contains(
          edge.child
          )
       ) {

      recovered.edges
        .push_back(
            edge
            );
    }
  }

  recovered.next_node_id =
    max_id + 1;

  recovered
    .nodes.at(root_id)
    .qbf =
    std::make_shared<ResidualQBF>(root);

  std::queue<NodeID>
    reconstruct_queue;

  reconstruct_queue.push(
      root_id
      );

  while (
      !reconstruct_queue.empty()
      ) {

    NodeID parent =
      reconstruct_queue.front();

    reconstruct_queue.pop();

    auto parent_qbf =
      recovered
      .nodes[parent]
      .qbf;

    for (
        const auto& edge
        : outgoing[parent]
        ) {

      auto& child =
        recovered
        .nodes[
        edge.child
        ];

      if (
          child.qbf
         ) {
        continue;
      }

      child.qbf =
        std::make_shared<
        ResidualQBF
        >(
            brancher->branch(
              *parent_qbf,
              edge.variable,
              edge.value
              )
         );

      reconstruct_queue.push(
          edge.child
          );
    }
  }

  for (
      const auto& [id, node]
      : recovered.nodes
      ) {

    switch (
        node.status
        ) {

      case NodeStatus::LEAF_TAUT:
      case NodeStatus::LEAF_FQBF:
      case NodeStatus::EXPANDED:
        cache->insert(
            canonicalizer
            ->canonicalize(
              *node.qbf
              ).key,
            CacheEntry(
              id,
              node.multiplier_exponent
              )
            );

        break;

      default:
        break;
    }
  }

  state =
    std::move(
        recovered
        );
}
