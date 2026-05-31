#include "cache/SemanticCache.hpp"

bool SemanticCache::contains(
    const std::string& key
    ) const {
  return value_cache.find(key)
    != value_cache.end();
}

SolverResult SemanticCache::lookup(
    const std::string& key
    ) const {
  return value_cache.at(key);
}

void SemanticCache::insert(
    const std::string& key,
    SolverResult result
    ) {
  value_cache[key] = result;
}
