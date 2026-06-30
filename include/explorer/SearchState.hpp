#pragma once

#include <atomic>
#include <unordered_map>
#include <vector>

#include "explorer/SearchNode.hpp"
#include "explorer/SearchEdge.hpp"

static NodeID root_id = 1;

class SearchState {
  public:

    std::unordered_map<
      NodeID,
      SearchNode
        > nodes;

    std::vector<SearchEdge> edges;

    NodeID next_node_id {root_id};

    NodeID allocate_node_id();
}; // class SearchState
