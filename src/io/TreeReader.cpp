#include "io/TreeReader.hpp"

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#include "explorer/NodeStatus.hpp"

namespace {

  struct PendingNode {

    SearchNode node;

    bool seen_depth = false;

    bool seen_hash = false;

    bool seen_status = false;

    bool seen_value = false;
  };
}

TreeReader::TreeReader(
    const std::string& filename
    )
  : filename_(filename)
{
}

void
TreeReader::read(
    SearchState& state,
    ResidualQBF& root,
    GameBrancher* brancher,
    SemanticCache* cache
    ) const {
  std::ifstream in(filename_);

  if (!in) {
    return;
  }

  struct PendingNode {

    SearchNode node;

    bool seen_depth = false;

    bool seen_status = false;

    bool seen_value = false;
  };

  std::unordered_map<
    NodeID,
    PendingNode
      > pending_nodes;

  std::vector<SearchEdge>
    pending_edges;

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
        token == "UPDATE"
       ) {

      NodeID id;

      ss >> id;

      auto it =
        pending_nodes.find(
            id
            );

      if (
          it ==
          pending_nodes.end()
         ) {
        continue;
      }

      if (
          !std::getline(
            in,
            line
            )
         ) {
        break;
      }

      {
        std::stringstream value_ss(
            line
            );

        std::string label;

        value_ss >> label;

        if (
            label == "VALUE"
           ) {

          value_ss
            >> it->second
            .node
            .value;

          it->second
            .seen_value = true;
        }
      }

      if (
          !std::getline(
            in,
            line
            )
         ) {
        break;
      }

      {
        std::stringstream status_ss(
            line
            );

        std::string label;
        std::string status;

        status_ss
          >> label
          >> status;

        if (
            label == "STATUS"
           ) {

          it->second
            .node
            .status =
            common::parse_status(
                status
                );

          it->second
            .seen_status = true;
        }
      }

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
        token == "HASH"
       ) {

      std::string hash;

      std::getline(
          ss,
          hash
          );

      if (
          !hash.empty()
          &&
          hash.front()
          == ' '
         ) {

        hash.erase(
            0,
            1
            );
      }

      node.node
        .canonical_hash =
        hash;

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

  std::unordered_set<NodeID>
    complete_nodes;

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
       ) {

      complete_nodes
        .insert(id);
    }
  }

  if (
      !complete_nodes.contains(
        0
        )
     ) {
    return;
  }

  std::unordered_map<
    NodeID,
    std::vector<SearchEdge>
      > outgoing;

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

  std::unordered_set<NodeID>
    reachable;

  std::queue<NodeID> q;

  q.push(0);

  reachable.insert(0);

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

  SearchState recovered;

  NodeID max_id = 0;

  for (
      NodeID id
      : reachable
      ) {

    recovered.nodes[id] =
      pending_nodes[id]
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
    .nodes[0]
    .qbf =
    std::make_shared<ResidualQBF>(root);

  std::queue<NodeID>
    reconstruct_queue;

  reconstruct_queue.push(
      0
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

    if (
        node.canonical_hash.empty()
       ) {
      continue;
    }

    switch (
        node.status
        ) {

      case NodeStatus::CACHE_HIT:
      case NodeStatus::LEAF_TAUT:
      case NodeStatus::LEAF_FQBF:
      case NodeStatus::LEAF_COMPLETE:

        cache->insert(
            node.canonical_hash,
            node.value
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
