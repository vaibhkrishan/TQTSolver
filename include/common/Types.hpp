#pragma once

#include <cstdint>
#include <boost/multiprecision/gmp.hpp>

using ExponentType = uint64_t;

using VariableID = uint64_t;
using Literal = int64_t;
using NodeID = uint64_t;

enum class QuantifierType {
  EXISTS,
  FORALL
};
