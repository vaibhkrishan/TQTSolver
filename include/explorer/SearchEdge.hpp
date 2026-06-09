#pragma once

#include "explorer/NodeStatus.hpp"
#include "common/Types.hpp"

struct SearchEdge {

  NodeID parent;

  NodeID child;

  VariableID variable;

  bool value;
}; // struct SearchEdge
