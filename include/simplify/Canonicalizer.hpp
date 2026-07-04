#pragma once

#include <string>

#include "formula/ResidualQBF.hpp"

struct CanonicalInfo {
  std::string key;
  ExponentType multiplier_exponent;

  CanonicalInfo(std::string key,
      ExponentType multiplier_exponent):
    key(key),
    multiplier_exponent(multiplier_exponent)
  {}
};

class Canonicalizer {
  public:
    CanonicalInfo canonicalize(
        const ResidualQBF& qbf
        );
};
