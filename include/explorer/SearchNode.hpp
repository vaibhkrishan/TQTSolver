#pragma once

#include <string>

#include "explorer/NodeStatus.hpp"
#include "formula/ResidualQBF.hpp"
#include "common/Result.hpp"

struct SearchNode {

  NodeID id;

  std::size_t depth;

  std::string canonical_hash;

  NodeStatus status;
  SolverResult value {0};

  std::shared_ptr<ResidualQBF> qbf;
}; // struct SearchNode
