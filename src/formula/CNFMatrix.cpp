#include "formula/CNFMatrix.hpp"

CNFMatrix CNFMatrix::restrict_literal(
    Literal lit
    ) const {
  return *this;
}

bool CNFMatrix::empty() const {
  return clauses.empty();
}

bool CNFMatrix::contains_empty_clause() const {
  for (const auto& clause : clauses) {
    if (clause.empty()) {
      return true;
    }
  }
  return false;
}
