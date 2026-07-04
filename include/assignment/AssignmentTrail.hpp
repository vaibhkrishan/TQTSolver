#pragma once

#include <memory>

#include "common/Types.hpp"

class AssignmentTrail {
public:
  std::shared_ptr<const AssignmentTrail> parent;
  VariableID variable;
  bool value;

  AssignmentTrail();
  AssignmentTrail(
      std::shared_ptr<const AssignmentTrail> p,
      VariableID v,
      bool val
      );
};
