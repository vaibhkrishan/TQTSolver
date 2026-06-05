#include "simplify/Canonicalizer.hpp"

#include <sstream>

std::string
Canonicalizer::canonicalize(
    const ResidualQBF& qbf
    ) {

  const auto& formula = qbf.materialize();
  std::unordered_map<int, int> renaming;

  int next_id = 1;

  auto get_canonical_var =
    [&](int var) {

      if (
          renaming.find(var)
          == renaming.end()
         ) {

        renaming[var] = next_id++;
      }

      return renaming[var];
    };

  std::stringstream out;

  for (const auto& block : formula.prefix) {

    if (
        block.type ==
        QuantifierType::EXISTS
       ) {
      out << "E ";
    }
    else {
      out << "A ";
    }

    for (int var : block.variables) {

      out
        << get_canonical_var(var)
        << " ";
    }

    out << "| ";
  }

  for (
      const auto& clause :
      formula.matrix.clauses
      ) {

    out << "(";

    for (
        size_t i = 0;
        i < clause.size();
        ++i
        ) {

      int lit = clause[i];

      int var = std::abs(lit);

      int canon =
        get_canonical_var(var);

      if (lit < 0) {
        out << "-";
      }

      out << canon;

      if (i + 1 < clause.size()) {
        out << " ";
      }
    }

    out << ")";
  }

  return out.str();
}
