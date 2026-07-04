#include "util/QDIMACSParser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <cstdlib>

std::shared_ptr<QBFFormula>
QDIMACSParser::parse(const std::string& filename) {

  std::ifstream input(filename);
  if (!input.is_open()) {
    throw std::runtime_error("Could not open QDIMACS file.");
  }

  std::string line;

  bool seen_problem = false;
  bool reading_clauses = false;

  int declared_clauses = 0;
  int declared_vars = 0;

  std::vector<QuantifierBlock> prefix;
  std::unordered_set<VariableID> prefix_vars;

  std::vector<std::vector<Literal>> clauses;
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
      parse_problem_line(line, declared_vars, declared_clauses);
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

      parse_quantifier_line(line, prefix, prefix_vars);
      continue;
    }

    // clause line
    reading_clauses = true;

    if (clauses.size() >= declared_clauses) {
      throw std::runtime_error(
          "More clauses than declared in problem line."
          );
    }

    parse_clause_line(line, clauses, clause_vars);
  }

  // ---- FINAL CHECKS ----

  if (clauses.size() < declared_clauses) {
    throw std::runtime_error(
        "Expected " + std::to_string(declared_clauses) +
        " clauses but found " + std::to_string(clauses.size())
        );
  }

  // add missing variables as outer existential block
  std::vector<VariableID> missing;

  for (VariableID v : clause_vars) {
    if (!prefix_vars.contains(v)) {
      missing.push_back(v);
    }
  }

  if (!missing.empty()) {
    QuantifierBlock block(QuantifierType::EXISTS, missing);

    prefix.insert(prefix.begin(), block);
  }

  auto formula = std::make_shared<QBFFormula>();

  std::unordered_map<
    VariableID,
    VariableID
      > old_to_new;

  VariableID next = 1;

  for (auto& block : prefix) {

    QuantifierBlock new_block(block.get_type());

    for (VariableID old : block.get_variables()) {

      old_to_new[old] = next;

      new_block.add_variable(next);

      ++next;
    }

    formula->prefix.push_back(
        std::move(new_block)
        );
  }

  formula->num_variables = next - 1;

  for (const auto& clause : clauses) {

    std::vector<Literal> new_clause;

    for (Literal lit : clause) {

      VariableID var =
        old_to_new.at(
            std::abs(lit)
            );

      new_clause.push_back(
          lit < 0
          ? -var
          : var
          );
    }

    formula->matrix.clauses.push_back(
        std::move(new_clause)
        );
  }
  return formula;
}

void QDIMACSParser::parse_problem_line(
    const std::string& line,
    int& declared_variables,
    int& declared_clauses
    ) {
  std::stringstream ss(line);

  std::string p, cnf;

  ss >> p >> cnf >> declared_variables >> declared_clauses;

  if (p != "p" || cnf != "cnf") {
    throw std::runtime_error("Invalid problem line.");
  }
}

void QDIMACSParser::parse_quantifier_line(
    const std::string& line,
    std::vector<QuantifierBlock>& prefix,
    std::unordered_set<VariableID>& prefix_vars
    ) {
  QuantifierBlock block(parse_quantifier_type(line[0]));

  std::stringstream ss(line.substr(1));

  VariableID var;

  while (ss >> var) {

    if (var == 0) break;

    if (var < 1) {
      throw std::runtime_error("Variable out of bounds in prefix.");
    }

    if (!prefix_vars.insert(var).second) {
      throw std::runtime_error("Duplicate variable in prefix.");
    }

    block.add_variable(var);
  }

  prefix.push_back(block);
}

void QDIMACSParser::parse_clause_line(
    const std::string& line,
    std::vector<std::vector<Literal>>& clauses,
    std::unordered_set<VariableID>& clause_vars
    ) {
  std::stringstream ss(line);

  std::vector<Literal> clause;
  Literal lit;

  while (ss >> lit) {

    if (lit == 0) break;

    VariableID v = std::abs(lit);

    if (v < 1) {
      throw std::runtime_error("Variable out of bounds in clause.");
    }

    clause_vars.insert(v);
    clause.push_back(lit);
  }

  clauses.push_back(clause);
}

QuantifierType QDIMACSParser::parse_quantifier_type(char c) {
  if (c == 'e') return QuantifierType::EXISTS;
  if (c == 'a') return QuantifierType::FORALL;
  throw std::runtime_error("Unknown quantifier type: " + std::string(1, c));
}
