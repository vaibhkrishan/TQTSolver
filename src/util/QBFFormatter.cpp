#include "util/QBFFormatter.hpp"

#include <sstream>

std::string
QBFFormatter::format(
    const QBFFormula& formula
    ) {
  std::stringstream out;
  out << "Variables: "
    << formula.num_variables
    << "\n";
  out << "\nPrefix:\n";
  out << format_prefix(formula);
  out << "\nClauses:\n";
  out << format_clauses(formula);
  return out.str();
}


std::string
QBFFormatter::format_prefix(
    const QBFFormula& formula
    ) {
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
      out << var << " ";
    }
    out << "\n";
  }
  return out.str();
}


std::string
QBFFormatter::format_clauses(
    const QBFFormula& formula
    ) {
  std::stringstream out;
  for (const auto& clause : formula.matrix.clauses) {
    out << "(";
    for (
        size_t i = 0;
        i < clause.size();
        ++i
        ) {
      out << clause[i];
      if (i + 1 < clause.size()) {
        out << " v ";
      }
    }
    out << ")\n";
  }
  return out.str();
}
