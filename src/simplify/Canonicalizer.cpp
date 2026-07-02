#include "simplify/Canonicalizer.hpp"

#include <sstream>

QBFFormula Canonicalizer::canonicalize_formula(const QBFFormula& formula)
{
//actually rewrites variables
//produces a qsole-ready formula

    QBFFormula out;

    std::unordered_map<int,int> renaming;
    int next_id = 1;

    auto get = [&](int v) -> int {
        if (!renaming.count(v))
            renaming[v] = next_id++;
        return renaming[v];
    };

    // ------------------------
    // 1. Canonical prefix
    // ------------------------
    for (const auto& block : formula.prefix)
    {
        QuantifierBlock new_block;
        new_block.type = block.type;

        for (int v : block.variables)
            new_block.variables.push_back(get(v));

        out.prefix.push_back(std::move(new_block));
    }

    // ------------------------
    // 2. Canonical matrix
    // ------------------------
    for (const auto& clause : formula.matrix.clauses)
    {
        std::vector<Literal> new_clause;

        for (Literal lit : clause)
        {
            int var = std::abs(lit);
            int mapped = get(var);

            new_clause.push_back(lit > 0 ? mapped : -mapped);
        }

        out.matrix.clauses.push_back(std::move(new_clause));
    }

    // ------------------------
    // 3. num_variables
    // ------------------------
    out.num_variables = renaming.size();

    return out;
}

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
        block.type ==
        QuantifierType::EXISTS
       ) {
      out << "E-";
    }
    else {
      out << "A-";
      universals_seen++;
    }

    for (int var : block.variables) {

      if (!var_seen.contains(var) and block.type == QuantifierType::EXISTS) {
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
