#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "formula/QBFFormula.hpp"

class QDIMACSParser {
  public:

    std::shared_ptr<QBFFormula>
      parse(const std::string&);

  private:

    void parse_problem_line(
        const std::string&,
        int&,
        int&
        );

    void parse_quantifier_line(
        const std::string&,
        std::vector<QuantifierBlock>&,
        std::unordered_set<VariableID>&
        );

    void parse_clause_line(
        const std::string&,
        std::vector<std::vector<Literal>>&,
        std::unordered_set<VariableID>&
        );

    QuantifierType parse_quantifier_type(char);
};
