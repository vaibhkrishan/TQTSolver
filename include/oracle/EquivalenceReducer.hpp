#pragma once

#include "../formula/ResidualQBF.hpp"

class EquivalenceReducer {
  public:
    virtual ResidualQBF build_instance(
        const ResidualQBF& left,
        const ResidualQBF& right
        ) = 0;
    virtual ~EquivalenceReducer() = default;
}; // class EquivalenceReducer
