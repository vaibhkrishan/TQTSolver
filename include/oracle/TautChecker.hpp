#pragma once

#include "../formula/ResidualQBF.hpp"

class TautChecker {
  public:
    virtual bool is_taut(
        const ResidualQBF& qbf
        ) = 0;
    virtual ~TautChecker() = default;
}; // class TautChecker
