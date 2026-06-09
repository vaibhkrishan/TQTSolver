#include "io/TreeWriter.hpp"

#include <fstream>

namespace {

  std::string
    status_to_string(
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

}

TreeWriter::TreeWriter(
    const std::string& filename
    )
  : filename_(filename)
{
}

void
TreeWriter::capture(
    const SearchState& state
    ) {

  buffer_
    << "=== SNAPSHOT ===\n";

  for (const auto& [id, node] :
      state.nodes) {

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

  for (const auto& edge :
      state.edges) {

    buffer_
      << "EDGE "
      << edge.parent
      << " "
      << edge.child
      << " "
      << edge.variable
      << "="
      << edge.value
      << "\n";
  }

  buffer_ << "\n";

  if (
      buffered_bytes()
      > 10 * 1024 * 1024
     ) {
    flush();
  }
}

void
TreeWriter::flush() {

  std::ofstream out(
      filename_,
      std::ios::app
      );

  out << buffer_.str();

  buffer_.str("");

  buffer_.clear();
}

std::size_t
TreeWriter::buffered_bytes() {

  return static_cast<std::size_t>(
      buffer_.tellp()
      );
}
