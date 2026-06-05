#pragma once

#include <memory>
#include <string>

#include "QBFFormula.hpp"
#include "../assignment/AssignmentTrail.hpp"

class ResidualQBF {
public:
  std::shared_ptr<const QBFFormula> formula;
  std::shared_ptr<const AssignmentTrail> trail;
  mutable std::shared_ptr<QBFFormula> restricted_formula;
  mutable std::string canonical_hash;

  const QBFFormula& materialize() const;
  uint64_t count_tautological_strategies() const;
};
