#include "explorer/NodeStatus.hpp"

#include <iostream>

namespace common {

  std::string
    status_to_string(
        NodeStatus status
        ) {

      switch (status) {

        case NodeStatus::OPEN:
          return "OPEN";

        case NodeStatus::EXPANDED:
          return "EXPANDED";

        case NodeStatus::CACHE_HIT:
          return "CACHE_HIT";

        case NodeStatus::LEAF_TAUT:
          return "LEAF_TAUT";

        case NodeStatus::LEAF_FQBF:
          return "LEAF_FQBF";

        case NodeStatus::LEAF_COMPLETE:
          return "LEAF_COMPLETE";
      }

      return "UNKNOWN";
    }

  NodeStatus
    parse_status(
        const std::string& s
        ) {

      if (s == "OPEN") {
        return NodeStatus::OPEN;
      }

      if (s == "EXPANDED") {
        return NodeStatus::EXPANDED;
      }

      if (s == "CACHE_HIT") {
        return NodeStatus::CACHE_HIT;
      }

      if (s == "LEAF_TAUT") {
        return NodeStatus::LEAF_TAUT;
      }

      if (s == "LEAF_FQBF") {
        return NodeStatus::LEAF_FQBF;
      }

      std::cout << "str for status: " << s << std::endl;
      return NodeStatus::LEAF_COMPLETE;
    }

}
