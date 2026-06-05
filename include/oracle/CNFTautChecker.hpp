#pragma once

#include "oracle/TautChecker.hpp"
#include "formula/ResidualQBF.hpp"

class CNFTautChecker : public TautChecker {
  public:
    virtual bool is_taut(
        const ResidualQBF& qbf
        );
    virtual ~CNFTautChecker() = default;
}; // class CNFTautChecker
