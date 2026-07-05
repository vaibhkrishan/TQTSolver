#include "hashing/Canonicalizer.hpp"

#include <sstream>

std::string
Canonicalizer::canonicalize(
    const ResidualQBF& qbf
    ) {

  std::unordered_map<
    VariableID,
    VariableID
      > renaming;

  VariableID next_id = 1;

  std::stringstream out;

  // Prefix
  for (
      size_t i = qbf.modifier.current_quantifier_block;
      i < qbf.formula->prefix.size();
      ++i
      ) {

    const auto& block =
      qbf.formula->prefix[i];

    std::vector<VariableID>
      active_variables;

    for (
        VariableID var
        : block.get_variables()
        ) {

      if (
          qbf.modifier.variable_status[var]
          != VariableStatus::ACTIVE
         ) {
        continue;
      }

      renaming[var] = next_id;
      active_variables.push_back(next_id);

      ++next_id;
    }

    if (
        active_variables.empty()
       ) {
      continue;
    }

    out
      << (
          block.get_type()
          == QuantifierType::EXISTS
          ? "E "
          : "A "
         );

    for (
        VariableID var
        : active_variables
        ) {

      out
        << var
        << " ";
    }

    out << "| ";
  }

  // Matrix
  const auto& clauses =
    qbf.formula->matrix.get_clauses();

  for (
      size_t i = 0;
      i < clauses.size();
      ++i
      ) {

    if (
        !qbf.modifier.active_clauses[i]
       ) {
      continue;
    }

    out << "(";

    bool first = true;

    for (
        Literal lit
        : clauses[i]
        ) {

      VariableID var =
        std::abs(lit);

      if (
          qbf.modifier.variable_status[var]
          != VariableStatus::ACTIVE
         ) {
        continue;
      }

      if (!first) {
        out << " ";
      }

      first = false;

      if (lit < 0) {
        out << "-";
      }

      out << renaming.at(var);
    }

    out << ")";
  }

  return
    out.str();
}
