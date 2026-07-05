#pragma once

#include <vector>
#include <memory>

#include "Simplifier.hpp"

class SimplificationPipeline {
  public:
    SimplificationPipeline();
    std::vector<std::unique_ptr<Simplifier>> passes;

    void run(ResidualQBF& qbf);
}; // class SimplificationPipeline
