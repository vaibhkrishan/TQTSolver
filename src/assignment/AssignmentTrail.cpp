#include "assignment/AssignmentTrail.hpp"

AssignmentTrail::AssignmentTrail(): parent(nullptr),
  variable(0),
  value(false) {
  }

AssignmentTrail::AssignmentTrail(
    std::shared_ptr<const AssignmentTrail> p,
    VariableID v,
    bool val
    )
  : parent(p),
  variable(v),
  value(val) {
  }
