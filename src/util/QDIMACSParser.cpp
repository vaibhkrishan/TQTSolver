#include "util/QDIMACSParser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::shared_ptr<QBFFormula>
QDIMACSParser::parse(
    const std::string& filename
    ) {

  auto formula =
    std::make_shared<QBFFormula>();

  std::ifstream input(filename);

  if (!input.is_open()) {
    throw std::runtime_error(
        "Could not open QDIMACS file."
        );
  }

  std::string line;

  while (std::getline(input, line)) {

    if (line.empty()) {
      continue;
    }

    char first = line[0];

    if (first == 'c') {
      continue;
    }

    if (first == 'p') {

      parse_problem_line(
          line,
          *formula
          );

      continue;
    }

    if (
        first == 'e' ||
        first == 'a'
       ) {

      parse_quantifier_line(
          line,
          *formula
          );

      continue;
    }

    parse_clause_line(
        line,
        *formula
        );
  }

  return formula;
}

void QDIMACSParser::parse_problem_line(
    const std::string& line,
    QBFFormula& formula
    ) {

  std::stringstream ss(line);

  std::string p;
  std::string cnf;

  int num_clauses;

  ss >> p;
  ss >> cnf;
  ss >> formula.num_variables;
  ss >> num_clauses;
}

void QDIMACSParser::parse_quantifier_line(
    const std::string& line,
    QBFFormula& formula
    ) {

  QuantifierBlock block;

  block.type =
    parse_quantifier_type(
        line[0]
        );

  std::stringstream ss(
      line.substr(1)
      );

  int var;

  while (ss >> var) {

    if (var == 0) {
      break;
    }

    block.variables.push_back(var);
  }

  formula.prefix.push_back(block);
}

void QDIMACSParser::parse_clause_line(
    const std::string& line,
    QBFFormula& formula
    ) {

  std::vector<int> clause;

  std::stringstream ss(line);

  int lit;

  while (ss >> lit) {

    if (lit == 0) {
      break;
    }

    clause.push_back(lit);
  }

  formula.matrix.clauses.push_back(clause);
}

QuantifierType
QDIMACSParser::parse_quantifier_type(
    char c
    ) {

  if (c == 'e') {
    return QuantifierType::EXISTS;
  }

  return QuantifierType::FORALL;
}

std::vector<int>
QDIMACSParser::parse_integer_sequence(
    const std::string& line
    ) {

  std::vector<int> result;

  std::stringstream ss(line);

  int x;

  while (ss >> x) {

    if (x == 0) {
      break;
    }

    result.push_back(x);
  }

  return result;
}
