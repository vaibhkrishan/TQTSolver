#pragma once

#include <string>

#include "../formula/ResidualQBF.hpp"

class Canonicalizer {
  public:
    std::string canonicalize(
        const ResidualQBF& qbf
        );
};
