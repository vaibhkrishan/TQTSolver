#pragma once

#include <unordered_map>
#include <string>

#include "common/Result.hpp"
#include "common/Types.hpp"

class SemanticCache {
  public:
    std::unordered_map<std::string, NodeID> value_cache;

    bool contains(
        const std::string& key
        ) const;
    NodeID lookup(
        const std::string& key
        ) const;
    void insert(
        const std::string& key,
        NodeID node_id
        );
}; // class SemanticCache
