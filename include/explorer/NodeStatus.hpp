#pragma once

#include <cstdint>
#include <string>

using NodeID = std::uint64_t;

enum class NodeStatus {
  OPEN,
  EXPANDED,
  CACHE_HIT,
  LEAF_TAUT,
  LEAF_FQBF,
  LEAF_COMPLETE
}; // enum class NodeStatus

namespace common {
  std::string
    status_to_string(
        NodeStatus status
        );

  NodeStatus
    parse_status(
        const std::string& s
        );
}

