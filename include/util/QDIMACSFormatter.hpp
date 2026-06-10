#pragma once

#include <string>

#include "formula/QBFFormula.hpp"

class QDIMACSFormatter {
  public:

    static std::string format(
        const QBFFormula& formula
        );
}; // class QDIMACSFormatter
