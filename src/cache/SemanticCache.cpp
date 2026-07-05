#include "cache/SemanticCache.hpp"

bool SemanticCache::contains(
    const std::string& key
    ) const {
  return value_cache.find(key)
    != value_cache.end();
}

NodeID SemanticCache::lookup(
    const std::string& key
    ) const {
  return value_cache.at(key);
}

void SemanticCache::insert(
    const std::string& key,
    NodeID node_id
    ) {
  value_cache[key] = node_id;
}
