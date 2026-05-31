#pragma once

#include <unordered_map>
#include <string>

#include "../common/Result.hpp"

class SemanticCache {
  public:
    std::unordered_map<std::string, SolverResult> value_cache;

    bool contains(
        const std::string& key
        ) const;
    SolverResult lookup(
        const std::string& key
        ) const;
    void insert(
        const std::string& key,
        SolverResult result
        );
}; // class SemanticCache
