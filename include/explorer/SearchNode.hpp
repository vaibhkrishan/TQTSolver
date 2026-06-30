#pragma once

#include <string>

#include "explorer/NodeStatus.hpp"
#include "formula/ResidualQBF.hpp"
#include "common/Result.hpp"

struct SearchNode {

  NodeID id;

  std::size_t depth;

  NodeID equivalent_node {0};
  ExponentType multiplier_exponent {0};

  NodeStatus status;
  SolverResult value {0};

  std::shared_ptr<ResidualQBF> qbf;
}; // struct SearchNode
