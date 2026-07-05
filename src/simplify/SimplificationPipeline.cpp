#include "simplify/SimplificationPipeline.hpp"
#include "simplify/ClauseSimplifier.hpp"
#include "simplify/UnusedVariableSimplifier.hpp"

SimplificationPipeline::SimplificationPipeline() {
  passes.emplace_back(std::make_unique<ClauseSimplifier>());
  passes.emplace_back(std::make_unique<UnusedVariableSimplifier>());
}

void SimplificationPipeline::run(
    ResidualQBF& qbf
    ) {
  for (auto& pass : passes) {
    pass->apply(qbf);
  }
}
