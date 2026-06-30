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

        case NodeStatus::SEEN:
          return "SEEN";

        case NodeStatus::LEAF_TAUT:
          return "LEAF_TAUT";

        case NodeStatus::LEAF_FQBF:
          return "LEAF_FQBF";
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

      if (s == "SEEN") {
        return NodeStatus::SEEN;
      }

      if (s == "LEAF_TAUT") {
        return NodeStatus::LEAF_TAUT;
      }

      if (s == "LEAF_FQBF") {
        return NodeStatus::LEAF_FQBF;
      }

      std::cout << "Unknown status: " << s << std::endl;
      return NodeStatus::OPEN;
    }
}
