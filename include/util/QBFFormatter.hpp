#pragma once

#include <string>

#include "formula/QBFFormula.hpp"

class QBFFormatter {
  public:
    static std::string format(
        const QBFFormula& formula
        );

  private:
    static std::string format_prefix(
        const QBFFormula& formula
        );
    static std::string format_clauses(
        const QBFFormula& formula
        );
};
