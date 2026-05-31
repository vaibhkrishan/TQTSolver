#pragma once

#include "../formula/ResidualQBF.hpp"

class Simplifier {
public:
  virtual void apply(
      ResidualQBF& qbf
      ) = 0;
  virtual ~Simplifier() = default;
}; // class Simplifier
