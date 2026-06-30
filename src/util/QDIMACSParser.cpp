#include "util/QDIMACSParser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <cstdlib>

std::shared_ptr<QBFFormula>
QDIMACSParser::parse(const std::string& filename) {

  auto formula = std::make_shared<QBFFormula>();

  std::ifstream input(filename);
  if (!input.is_open()) {
    throw std::runtime_error("Could not open QDIMACS file.");
  }

  std::string line;

  bool seen_problem = false;
  bool reading_clauses = false;

  int declared_clauses = 0;
  int clauses_read = 0;

  std::unordered_set<VariableID> prefix_vars;
  std::unordered_set<VariableID> clause_vars;

  while (std::getline(input, line)) {

    if (line.empty()) continue;

    char first = line[0];

    // comment
    if (first == 'c') continue;

    // problem line
    if (first == 'p') {

      if (seen_problem) {
        throw std::runtime_error("Multiple problem lines found.");
      }

      seen_problem = true;
      parse_problem_line(line, *formula, declared_clauses);
      continue;
    }

    if (!seen_problem) {
      throw std::runtime_error(
          "Non-comment line before problem line."
          );
    }

    // quantifiers
    if (first == 'e' || first == 'a') {

      if (reading_clauses) {
        throw std::runtime_error(
            "Quantifier block appears after clauses."
            );
      }

      parse_quantifier_line(line, *formula, prefix_vars);
      continue;
    }

    // clause line
    reading_clauses = true;

    if (clauses_read >= declared_clauses) {
      throw std::runtime_error(
          "More clauses than declared in problem line."
          );
    }

    parse_clause_line(line, *formula, clause_vars);
    clauses_read++;
  }

  // ---- FINAL CHECKS ----

  if (clauses_read != declared_clauses) {
    throw std::runtime_error(
        "Expected " + std::to_string(declared_clauses) +
        " clauses but found " + std::to_string(clauses_read)
        );
  }

  // add missing variables as outer existential block
  std::vector<VariableID> missing;

  for (VariableID v : clause_vars) {
    if (!prefix_vars.count(v)) {
      missing.push_back(v);
    }
  }

  if (!missing.empty()) {
    QuantifierBlock block;
    block.type = QuantifierType::EXISTS;
    block.variables = missing;

    formula->prefix.insert(formula->prefix.begin(), block);
  }

  return formula;
}

void QDIMACSParser::parse_problem_line(
    const std::string& line,
    QBFFormula& formula,
    int& declared_clauses
    ) {
  std::stringstream ss(line);

  std::string p, cnf;

  ss >> p >> cnf >> formula.num_variables >> declared_clauses;

  if (p != "p" || cnf != "cnf") {
    throw std::runtime_error("Invalid problem line.");
  }

  formula.matrix.clauses.clear();
  formula.matrix.clauses.reserve(declared_clauses);
}

void QDIMACSParser::parse_quantifier_line(
    const std::string& line,
    QBFFormula& formula,
    std::unordered_set<VariableID>& prefix_vars
    ) {
  QuantifierBlock block;

  block.type = parse_quantifier_type(line[0]);

  std::stringstream ss(line.substr(1));

  VariableID var;

  while (ss >> var) {

    if (var == 0) break;

    if (var < 1 || var > formula.num_variables) {
      throw std::runtime_error("Variable out of bounds in prefix.");
    }

    if (!prefix_vars.insert(var).second) {
      throw std::runtime_error("Duplicate variable in prefix.");
    }

    formula.set_quantifier_type(var, block.type);
    block.variables.push_back(var);
  }

  formula.prefix.push_back(block);
}

void QDIMACSParser::parse_clause_line(
    const std::string& line,
    QBFFormula& formula,
    std::unordered_set<VariableID>& clause_vars
    ) {
  std::stringstream ss(line);

  std::vector<Literal> clause;
  Literal lit;

  while (ss >> lit) {

    if (lit == 0) break;

    VariableID v = std::abs(lit);

    if (v < 1 || v > formula.num_variables) {
      throw std::runtime_error("Variable out of bounds in clause.");
    }

    clause_vars.insert(v);
    clause.push_back(lit);
  }

  formula.matrix.clauses.push_back(clause);
}

QuantifierType QDIMACSParser::parse_quantifier_type(char c) {
  if (c == 'e') return QuantifierType::EXISTS;
  return QuantifierType::FORALL;
}
