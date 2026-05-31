#include "simplify/SimplificationPipeline.hpp"

void SimplificationPipeline::run(
    ResidualQBF& qbf
    ) {
  for (auto& pass : passes) {
    pass->apply(qbf);
  }
}
