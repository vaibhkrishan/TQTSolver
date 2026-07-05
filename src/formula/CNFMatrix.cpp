#include "formula/CNFMatrix.hpp"

CNFMatrix::CNFMatrix(
    std::vector<std::vector<Literal>> clauses
)
    : clauses(std::move(clauses))
{
}

const std::vector<std::vector<Literal>>& CNFMatrix::get_clauses() const {
  return clauses;
}
