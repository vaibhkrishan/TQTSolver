#include "formula/ResidualQBF.hpp"

const QBFFormula& ResidualQBF::materialize() const {
  if (restricted_formula) {
    return *restricted_formula;
  }
  restricted_formula =
    std::make_shared<QBFFormula>(
        *formula
        );

  std::unordered_map<
    VariableID,
    bool
      > assignment;

  auto current = trail;

  while (current) {

    assignment[current->variable]
      = current->value;

    current = current->parent;
  }

  CNFMatrix new_matrix;

  for (
      const auto& clause :
      restricted_formula->matrix.clauses
      ) {

    bool satisfied = false;

    std::vector<Literal> new_clause;

    for (Literal lit : clause) {

      VariableID var =
        std::abs(lit);

      auto it =
        assignment.find(var);

      if (
          it == assignment.end()
         ) {

        new_clause.push_back(lit);

        continue;
      }

      bool assigned_value =
        it->second;

      bool lit_value =
        (lit > 0)
        ? assigned_value
        : !assigned_value;

      if (lit_value) {

        satisfied = true;

        break;
      }
    }

    if (!satisfied) {

      new_matrix.clauses.push_back(
          new_clause
          );
    }
  }

  restricted_formula->matrix =
    std::move(new_matrix);

  std::vector<QuantifierBlock>
    new_prefix;

  for (
      const auto& block :
      restricted_formula->prefix
      ) {

    QuantifierBlock new_block;

    new_block.type = block.type;

    for (VariableID var :
        block.variables) {

      if (
          assignment.find(var)
          == assignment.end()
         ) {

        new_block.variables
          .push_back(var);
      }
    }

    if (!new_block.variables.empty()) {
      new_prefix.push_back(
          new_block
          );
    }
  }

  restricted_formula->prefix =
    std::move(new_prefix);

  return *restricted_formula;
}

uint64_t ResidualQBF::count_tautological_strategies() const {

  const auto& formula = materialize();
  uint64_t total = 1;

  size_t universal_count = 0;

  for (const auto& block : formula.prefix) {

    if (
        block.type ==
        QuantifierType::FORALL
       ) {

      universal_count +=
        block.variables.size();

      continue;
    }

    for (
        size_t i = 0;
        i < block.variables.size();
        ++i
        ) {

      uint64_t strategies =
        (
         uint64_t(1)
         << universal_count
        );

      strategies =
        (
         uint64_t(1)
         << strategies
        );

      total *= strategies;
    }
  }

  return total;
}

