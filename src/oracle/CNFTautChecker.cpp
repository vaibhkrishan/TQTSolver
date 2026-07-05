#include "oracle/CNFTautChecker.hpp"

bool CNFTautChecker::is_taut(const ResidualQBF& qbf) {
  return qbf.is_cnf_empty();
}
