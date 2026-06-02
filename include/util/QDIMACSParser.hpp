#pragma once

#include <memory>
#include <string>

#include "formula/QBFFormula.hpp"

class QDIMACSParser {
  public:

    std::shared_ptr<QBFFormula>
      parse(
          const std::string& filename
          );

  private:

    void parse_problem_line(
        const std::string& line,
        QBFFormula& formula
        );

    void parse_quantifier_line(
        const std::string& line,
        QBFFormula& formula
        );

    void parse_clause_line(
        const std::string& line,
        QBFFormula& formula
        );

    QuantifierType parse_quantifier_type(
        char c
        );

    std::vector<int> parse_integer_sequence(
        const std::string& line
        );
};
