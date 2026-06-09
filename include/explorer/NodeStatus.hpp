#pragma once

#include <cstdint>

using NodeID = std::uint64_t;

enum class NodeStatus {
  OPEN,
  EXPANDED,
  LEAF_TAUT,
  LEAF_FQBF,
  LEAF_COMPLETE
}; // enum class NodeStatus
