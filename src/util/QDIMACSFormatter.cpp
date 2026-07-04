#include "util/QDIMACSFormatter.hpp"

#include <sstream>

std::string
QDIMACSFormatter::format(
    const QBFFormula& formula
    ) {

  std::stringstream out;

  out
    << "p cnf "
    << formula.num_variables
    << " "
    << formula.matrix.clauses.size()
    << "\n";

  for (const auto& block :
      formula.prefix) {

    out
      << (
          block.get_type() ==
          QuantifierType::EXISTS
          ? 'e'
          : 'a'
         )
      << " ";

    const auto& vars = block.get_variables();
    for (VariableID var :
        vars) {

      out
        << var
        << " ";
    }

    out
      << "0\n";
  }

  for (const auto& clause :
      formula.matrix.clauses) {

    for (Literal lit :
        clause) {

      out
        << lit
        << " ";
    }

    out
      << "0\n";
  }

  return out.str();
}
