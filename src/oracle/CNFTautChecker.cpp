#include "oracle/CNFTautChecker.hpp"

bool CNFTautChecker::is_taut(const ResidualQBF& qbf) {
  const auto& formula = qbf.materialize();
  return formula.matrix.empty();
}
