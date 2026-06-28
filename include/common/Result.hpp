#pragma once

#include <cstdint>
#include <boost/multiprecision/gmp.hpp>

using SolverResult = boost::multiprecision::mpz_int;

enum class OracleResult {
  TRUE_RESULT,
  FALSE_RESULT,
  UNKNOWN
}; // enum class SolverResult
