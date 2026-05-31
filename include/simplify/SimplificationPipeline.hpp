#pragma once

#include <vector>
#include <memory>

#include "Simplifier.hpp"

class SimplificationPipeline {
  public:
    std::vector<std::unique_ptr<Simplifier>> passes;

    void run(ResidualQBF& qbf);
}; // class SimplificationPipeline
