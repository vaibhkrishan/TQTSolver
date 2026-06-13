#pragma once

#include <string>

#include "explorer/SearchState.hpp"
#include "explorer/GameBrancher.hpp"

#include "cache/SemanticCache.hpp"

class TreeReader {
  public:

    explicit TreeReader(
        const std::string& filename
        );

    void read(
        SearchState&,
        ResidualQBF&,
        GameBrancher*,
        SemanticCache*
        ) const;

  private:

    std::string filename_;
}; // class TreeReader
