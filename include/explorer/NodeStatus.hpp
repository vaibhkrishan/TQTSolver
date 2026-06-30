#pragma once

#include <cstdint>
#include <string>

enum class NodeStatus {
  OPEN,
  EXPANDED,
  SEEN,
  LEAF_TAUT,
  LEAF_FQBF
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

