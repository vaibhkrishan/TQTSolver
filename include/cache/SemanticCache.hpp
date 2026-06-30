#pragma once

#include <unordered_map>
#include <string>

#include "common/Result.hpp"
#include "common/Types.hpp"

struct CacheEntry {
  NodeID node_id;
  ExponentType multiplier_exponent;
  CacheEntry(NodeID node_id,
      ExponentType multiplier_exponent
      ): 
  node_id(node_id),
  multiplier_exponent(multiplier_exponent) {
  }
  CacheEntry()
  {}
};

class SemanticCache {
  public:
    std::unordered_map<std::string, CacheEntry> value_cache;

    bool contains(
        const std::string& key
        ) const;
    CacheEntry lookup(
        const std::string& key
        ) const;
    void insert(
        const std::string& key,
        CacheEntry result
        );
}; // class SemanticCache
