#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "formula/QBFFormula.hpp"

class QDIMACSParser {
public:

    std::shared_ptr<QBFFormula>
        parse(const std::string& filename);

private:

    void parse_problem_line(
    const std::string& line,
    QBFFormula& formula,
    int& declared_clauses
);

    void parse_quantifier_line(
        const std::string& line,
        QBFFormula& formula,
        std::unordered_set<int>& prefix_vars
    );

    void parse_clause_line(
        const std::string& line,
        QBFFormula& formula,
        std::unordered_set<int>& clause_vars
    );

    QuantifierType parse_quantifier_type(char c);

    std::vector<int> parse_integer_sequence(
        const std::string& line
    );
};
