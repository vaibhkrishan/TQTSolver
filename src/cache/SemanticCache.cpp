#include "cache/SemanticCache.hpp"

bool SemanticCache::contains(
    const std::string& key
    ) const {
  return value_cache.find(key)
    != value_cache.end();
}

CacheEntry SemanticCache::lookup(
    const std::string& key
    ) const {
  return value_cache.at(key);
}

void SemanticCache::insert(
    const std::string& key,
    CacheEntry result
    ) {
  value_cache[key] = std::move(result);
}
