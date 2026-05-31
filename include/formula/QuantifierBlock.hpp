#pragma once

#include <vector>
#include "../common/Types.hpp"

enum class QuantifierType {
  EXISTS,
  FORALL
};

struct QuantifierBlock {
  QuantifierType type;
  std::vector<VariableID> variables;
};
