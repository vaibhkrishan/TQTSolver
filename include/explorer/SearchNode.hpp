#pragma once

#include <string>

#include "explorer/NodeStatus.hpp"
#include "formula/ResidualQBF.hpp"

struct SearchNode {

  NodeID id;

  std::size_t depth;

  std::string canonical_hash;

  NodeStatus status;

  std::shared_ptr<ResidualQBF> qbf;
}; // struct SearchNode
