#pragma once

#include <sstream>
#include <string>

#include "explorer/SearchState.hpp"

class TreeWriter {
  public:

    explicit TreeWriter(
        const std::string& filename
        );

    void capture(
        const SearchState& state
        );

    void flush();

    std::size_t
      buffered_bytes();

  private:

    std::string filename_;

    std::stringstream buffer_;
}; // class TreeWriter
