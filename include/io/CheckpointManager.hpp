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

    void print_initial_formula(
        const QBFFormula& qbf
        );

    void append_node(
        const SearchNode& node
        );

    void append_edge(
        const SearchEdge& edge
        );

    void update_status(
        NodeID id,
        NodeStatus status
        );

    void flush();

  private:

    std::string filename_;

    std::stringstream buffer_;

    static std::string status_to_string(
        NodeStatus status
        );
}; // class CheckpointManager
