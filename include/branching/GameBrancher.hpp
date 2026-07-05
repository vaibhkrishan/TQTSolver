#pragma once

#include "formula/ResidualQBF.hpp"

class GameBrancher {
  public:
    ResidualQBF branch(
        const ResidualQBF& qbf,
        VariableID variable,
        bool value
        );
}; // class GameBrancher
