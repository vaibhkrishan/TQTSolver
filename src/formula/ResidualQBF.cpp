#include "formula/ResidualQBF.hpp"

ResidualQBF::ResidualQBF(
    std::shared_ptr<const QBFFormula> formula
    )
  : formula(std::move(formula)),
  modifier(
      this->formula->matrix.clauses.size(),
      this->formula->num_variables
      )
{
}

VariableStatus ResidualQBF::get_variable_status(
    VariableID var) const {
  return modifier.variable_status[var];
}

QuantifierType ResidualQBF::get_quantifier_type (
    VariableID var) const {
  return formula->get_quantifier_type(var);
}

bool
ResidualQBF::is_cnf_empty() const {

  for (bool active : modifier.active_clauses) {
    if (active) {
      return false;
    }
  }

  return true;
}

bool
ResidualQBF::contains_empty_clause() const {

  const auto& clauses =
    formula->matrix.clauses;

  for (
      size_t i = 0;
      i < clauses.size();
      ++i
      ) {

    if (
        !modifier.active_clauses[i]
       ) {
      continue;
    }

    bool empty = true;

    for (
        Literal lit
        : clauses[i]
        ) {

      VariableID var =
        std::abs(lit);

      switch (
          modifier.variable_status[var]
          ) {

        case VariableStatus::ACTIVE:
          empty = false;
          break;

        case VariableStatus::INACTIVE:
        case VariableStatus::ASSIGNED_TRUE:
        case VariableStatus::ASSIGNED_FALSE:
          break;

        default:
          break;
      }

      if (!empty) {
        break;
      }
    }

    if (empty) {
      return true;
    }
  }

  return false;
}

bool
ResidualQBF::contains_pure_clause() const {

  const auto& clauses =
    formula->matrix.clauses;

  for (
      size_t i = 0;
      i < clauses.size();
      ++i
      ) {

    if (
        !modifier.active_clauses[i]
       ) {
      continue;
    }

    bool pure = true;
    bool has_literal = false;

    for (
        Literal lit
        : clauses[i]
        ) {

      VariableID var =
        std::abs(lit);

      if (
          modifier.variable_status[var]
          != VariableStatus::ACTIVE
         ) {
        continue;
      }

      has_literal = true;

      if (
          formula->get_quantifier_type(var)
          == QuantifierType::EXISTS
         ) {

        pure = false;
        break;
      }
    }

    if (
        has_literal &&
        pure
       ) {
      return true;
    }
  }

  return false;
}

bool
ResidualQBF::is_pure_existential() const {

  const auto& clauses =
    formula->matrix.clauses;

  for (
      size_t i = 0;
      i < clauses.size();
      ++i
      ) {

    if (
        !modifier.active_clauses[i]
       ) {
      continue;
    }

    for (
        Literal lit
        : clauses[i]
        ) {

      VariableID var =
        std::abs(lit);

      if (
          modifier.variable_status[var]
          != VariableStatus::ACTIVE
         ) {
        continue;
      }

      if (
          formula->get_quantifier_type(var)
          == QuantifierType::FORALL
         ) {
        return false;
      }
    }
  }

  return true;
}

bool ResidualQBF::is_prefix_empty() const {
  return modifier.current_quantifier_block == formula->prefix.size();
}

SolverResult
ResidualQBF::count_tautological_strategies() const {

  ExponentType exponent = 0;

  ExponentType dependency_size = 0;

  for (
      size_t i = modifier.current_quantifier_block;
      i < formula->prefix.size();
      ++i
      ) {

    const auto& block =
      formula->prefix[i];

    ExponentType active_variables = 0;

    for (
        VariableID var
        : block.get_variables()
        ) {

      if (
          modifier.variable_status[var]
          == VariableStatus::ACTIVE
         ) {

        ++active_variables;
      }
    }

    if (
        block.get_type()
        == QuantifierType::FORALL
       ) {

      dependency_size +=
        active_variables;
    }
    else {

      exponent +=
        active_variables
        * (ExponentType(1)
            << dependency_size);
    }
  }

  return SolverResult(1) << exponent;
}

std::unique_ptr<QBFFormula>
ResidualQBF::materialize() const {


  std::vector<QuantifierBlock> prefix;

  std::unordered_map<
    VariableID,
    VariableID
      > renaming;

  VariableID next_variable = 1;

  for (
      size_t i = modifier.current_quantifier_block;
      i < formula->prefix.size();
      ++i
      ) {

    const auto& old_block =
      formula->prefix[i];

    std::vector<VariableID>
      variables;

    for (
        VariableID var
        : old_block.get_variables()
        ) {

      if (
          modifier.variable_status[var]
          == VariableStatus::ASSIGNED_TRUE
          or
          modifier.variable_status[var]
          == VariableStatus::ASSIGNED_FALSE
         ) {
        continue;
      }

      renaming[var] =
        next_variable;

      variables.push_back(
          next_variable
          );

      ++next_variable;
    }

    if (
        !variables.empty()
       ) {

      prefix.emplace_back(
          old_block.get_type(),
          variables
          );
    }
 }

std::vector<
    std::vector<Literal>
    > clauses;

for (
    size_t i = 0;
    i < formula->matrix.clauses.size();
    ++i
    ) {

  if (!modifier.active_clauses[i]) {
    continue;
  }

  std::vector<Literal> clause;

  bool clause_satisfied = false;

  for (
      Literal lit
      : formula->matrix.clauses[i]
      ) {

    VariableID var = std::abs(lit);

    VariableStatus status =
        modifier.variable_status[var];

    
    if (status == VariableStatus::ASSIGNED_TRUE)
    {
      if (lit > 0)
      {
        clause_satisfied = true;
        break;
      }
      else
      {
        continue;
      }
    }

    if (status == VariableStatus::ASSIGNED_FALSE)
    {
      if (lit < 0)
      {
        clause_satisfied = true;
        break;
      }
      else
      {
        continue;
      }
    }

    
    if (status == VariableStatus::ACTIVE)
    {
      VariableID new_var =
          renaming.at(var);

      clause.push_back(
          lit > 0
          ? new_var
          : -new_var
          );
    }
  }

  if (clause_satisfied)
  {
    continue;
  }

  clauses.push_back(
      std::move(clause)
      );
}

  return std::make_unique<QBFFormula>(
      next_variable - 1,
      std::move(prefix),
      std::move(clauses)
      );
}
