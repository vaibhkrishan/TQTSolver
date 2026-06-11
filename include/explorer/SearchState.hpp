#pragma once

#include <atomic>
#include <unordered_map>
#include <vector>

#include "explorer/SearchNode.hpp"
#include "explorer/SearchEdge.hpp"

class SearchState {
  public:

    std::unordered_map<
      NodeID,
      SearchNode
        > nodes;

    std::vector<SearchEdge> edges;

    NodeID next_node_id {0};

    NodeID allocate_node_id();
}; // class SearchState
