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
    << "VALUE "
    << node.value
    << "\n";

  buffer_
    << "MULTIPLIER_EXPONENT "
    << node.multiplier_exponent
    << "\n";

  buffer_
    << "EQUIVALENT_NODE "
    << node.equivalent_node
    << "\n";

  buffer_
    << "STATUS "
    << common::status_to_string(
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
    << "\n\n";
}

void
CheckpointManager::update_node(
    const SearchNode& node
    ) {

  buffer_
    << "UPDATE "
    << node.id
    << "\n";

  buffer_
    << "VALUE "
    << node.value
    << "\n";

  buffer_
    << "MULTIPLIER_EXPONENT "
    << node.multiplier_exponent
    << "\n";

  buffer_
    << "EQUIVALENT_NODE "
    << node.equivalent_node
    << "\n";

  buffer_
    << "STATUS "
    << common::status_to_string(
        node.status
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
