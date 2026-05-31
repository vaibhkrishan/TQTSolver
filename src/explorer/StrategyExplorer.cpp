#include "explorer/StrategyExplorer.hpp"

SolverResult StrategyExplorer::recurse(
    ResidualQBF qbf
    ) {
  pipeline->run(qbf);
  std::string key =
    canonicalizer->canonicalize(qbf);
  if (cache->contains(key)) {
    return cache->lookup(key);
  }
  if (oracle->is_taut(qbf)) {
    cache->insert(
        key,
        SolverResult::TRUE_RESULT
        );
    return SolverResult::TRUE_RESULT;
  }

  if (oracle->is_fqbf(qbf)) {
    SolverResult result =
      oracle->solve_fqbf(qbf);
    cache->insert(key, result);
    return result;
  }
  VariableID variable =
    heuristic->choose_variable(qbf);
  ResidualQBF left =
    brancher->branch(
        qbf,
        variable,
        false
        );
  ResidualQBF right =
    brancher->branch(
        qbf,
        variable,
        true
        );
  if (oracle->equivalent(left, right)) {
    SolverResult result =
      recurse(left);
    cache->insert(key, result);
    return result;
  }
  SolverResult left_result =
    recurse(left);
  SolverResult right_result =
    recurse(right);
  SolverResult final_result;
  if (heuristic->is_existential(
        qbf,
        variable
        )) {
    final_result =
      combine_existential(
          left_result,
          right_result
          );
  }
  else {
    final_result =
      combine_universal(
          left_result,
          right_result
          );
  }
  cache->insert(
      key,
      final_result
      );
  return final_result;
}

SolverResult StrategyExplorer::combine_existential(
    SolverResult left,
    SolverResult right
    ) {
  if (
      left == SolverResult::TRUE_RESULT ||
      right == SolverResult::TRUE_RESULT
     ) {
    return SolverResult::TRUE_RESULT;
  }
  if (
      left == SolverResult::FALSE_RESULT &&
      right == SolverResult::FALSE_RESULT
     ) {
    return SolverResult::FALSE_RESULT;
  }
  return SolverResult::UNKNOWN;
}

SolverResult StrategyExplorer::combine_universal(
    SolverResult left,
    SolverResult right
    ) {
  if (
      left == SolverResult::FALSE_RESULT ||
      right == SolverResult::FALSE_RESULT
     ) {
    return SolverResult::FALSE_RESULT;
  }
  if (
      left == SolverResult::TRUE_RESULT &&
      right == SolverResult::TRUE_RESULT
     ) {
    return SolverResult::TRUE_RESULT;
  }
  return SolverResult::UNKNOWN;
}
