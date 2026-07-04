#pragma once

#include <vector>
#include "common/Types.hpp"

class CNFMatrix {
public:
  std::vector<std::vector<Literal>> clauses;
  CNFMatrix restrict_literal(Literal lit) const;
  bool empty() const;
  bool contains_empty_clause() const;
}; 
