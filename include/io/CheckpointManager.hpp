#pragma once

#include <sstream>
#include <string>

#include "explorer/SearchNode.hpp"
#include "explorer/SearchEdge.hpp"

#include "formula/ResidualQBF.hpp"

class CheckpointManager {
  public:

    explicit CheckpointManager(
        const std::string& filename
        );

    void append_node(
        const SearchNode& node
        );

    void append_edge(
        const SearchEdge& edge
        );

    void update_node(
        const SearchNode& node
        );

    void flush();

  private:

    std::string filename_;

    std::stringstream buffer_;
}; // class CheckpointManager
