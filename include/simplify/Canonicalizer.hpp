#pragma once

#include <string>
#include "formula/QBFFormula.hpp"
#include "formula/ResidualQBF.hpp"

struct CanonicalInfo {
  std::string key;
  ExponentType multiplier_exponent;

  CanonicalInfo(std::string key,
      ExponentType multiplier_exponent)
    : key(key), multiplier_exponent(multiplier_exponent)
  {}
};

class Canonicalizer {
public:
    static QBFFormula canonicalize_formula(const QBFFormula& formula);

    CanonicalInfo canonicalize(
        const ResidualQBF& qbf
    );
};
