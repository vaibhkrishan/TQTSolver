#include "explorer/StrategyExplorer.hpp"
#include "explorer/TrivialHeuristic.hpp"

#include <iostream>

StrategyExplorer::StrategyExplorer()
{
  oracle = new SemanticOracle;
  cache = new SemanticCache;
  pipeline = new SimplificationPipeline;
  canonicalizer = new Canonicalizer;
  heuristic = new TrivialHeuristic;
  brancher = new GameBrancher;
}

SolverResult StrategyExplorer::recurse(
    ResidualQBF qbf
    ) {
  pipeline->run(qbf);
  const auto& formula = qbf.materialize();
  std::string key =
    canonicalizer->canonicalize(qbf);

  if (cache->contains(key)) {
    return cache->lookup(key);
  }
  if (oracle->is_taut(qbf)) {
    auto tautCount = qbf.count_tautological_strategies();
    cache->insert(
        key,
        tautCount
        );
    return tautCount;
  }

  if (oracle->is_fqbf(qbf)) {
    cache->insert(key, 0); // return 0 for fqbf
    return 0;
  }
  VariableID variable =
    heuristic->choose_variable(qbf);
  std::cout << "variable " << variable << std::endl;
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
    final_result = left_result + right_result;
  }
  else {
    final_result = left_result * right_result;
  }
  cache->insert(
      key,
      final_result
      );
  return final_result;
}
