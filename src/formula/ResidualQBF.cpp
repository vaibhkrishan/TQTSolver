#include "formula/ResidualQBF.hpp"

const QBFFormula& ResidualQBF::materialize() const {
  if (restricted_formula) {
    return *restricted_formula;
  }
  restricted_formula =
    std::make_shared<QBFFormula>(
        *formula
        );
  return *restricted_formula;
}
