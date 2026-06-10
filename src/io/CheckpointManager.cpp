#include "io/CheckpointManager.hpp"

#include "util/QDIMACSFormatter.hpp"

#include <fstream>
#include <iostream>

CheckpointManager::CheckpointManager(
    const std::string& filename
    )
  : filename_(filename)
{
  
}

void CheckpointManager::print_initial_formula(
    const QBFFormula& qbf
    )
{

  std::ofstream out(
      filename_
      );

  out
    << QDIMACSFormatter::format(qbf)
    << "0\n"
    << std::endl;
  out.close();
}

void
CheckpointManager::append_node(
    const SearchNode& node
    ) {

  buffer_
    << "NODE "
    << node.id
    << "\n";

  buffer_
    << "DEPTH "
    << node.depth
    << "\n";

  buffer_
    << "HASH "
    << node.canonical_hash
    << "\n";

  buffer_
    << "STATUS "
    << status_to_string(
        node.status
        )
    << "\n\n";
}

void
CheckpointManager::append_edge(
    const SearchEdge& edge
    ) {

  buffer_
    << "EDGE "
    << edge.parent
    << " "
    << edge.child
    << " "
    << edge.variable
    << " "
    << edge.value
    << "\n";
}

void
CheckpointManager::update_status(
    NodeID id,
    NodeStatus status
    ) {

  buffer_
    << "UPDATE "
    << id
    << "\n";

  buffer_
    << "STATUS "
    << status_to_string(
        status
        )
    << "\n\n";
}

void
CheckpointManager::flush() {

  std::ofstream out(
      filename_,
      std::ios::app
      );

  out
    << buffer_.str();
  out.close();

  buffer_.str("");

  buffer_.clear();
}

std::string
CheckpointManager::status_to_string(
    NodeStatus status
    ) {

  switch (status) {

    case NodeStatus::OPEN:
      return "OPEN";

    case NodeStatus::EXPANDED:
      return "EXPANDED";

    case NodeStatus::LEAF_TAUT:
      return "LEAF_TAUT";

    case NodeStatus::LEAF_FQBF:
      return "LEAF_FQBF";

    case NodeStatus::LEAF_COMPLETE:
      return "LEAF_COMPLETE";
  }

  return "UNKNOWN";
}
