#include "simplify/Canonicalizer.hpp"

#include <sstream>

CanonicalInfo
Canonicalizer::canonicalize(
    const ResidualQBF& qbf
    ) {

  const auto& formula = qbf.materialize();
  std::unordered_map<int, int> renaming;
  std::unordered_set<int> var_seen;

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

  out << formula.num_variables << "|";
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
      var_seen.insert(var);

      int canon =
        get_canonical_var(var);

      if (lit < 0) {
        out << "-";
      }

      out << canon;

      if (i + 1 < clause.size()) {
        out << "+";
      }
    }

    out << ")";
  }

  ExponentType multiplier_exponent = 0;
  ExponentType universals_seen = 0;


  out << "|";
  for (const auto& block : formula.prefix) {

    if (
        block.get_type() ==
        QuantifierType::EXISTS
       ) {
      out << "E-";
    }
    else {
      out << "A-";
      universals_seen++;
    }

    const auto& vars = block.get_variables();
    for (int var : vars) {

      if (!var_seen.contains(var) and block.get_type() == QuantifierType::EXISTS) {
        multiplier_exponent += (static_cast<ExponentType>(1) << universals_seen);
        continue;
      }
      out
        << get_canonical_var(var)
        << "_";
    }
  }
  return CanonicalInfo(out.str(), multiplier_exponent);
}
