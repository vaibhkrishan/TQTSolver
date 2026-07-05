#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "formula/QuantifierBlock.hpp"
#include "formula/CNFMatrix.hpp"

class QBFFormula {
public:
    QBFFormula(
        size_t num_variables,
        std::vector<QuantifierBlock> prefix,
        std::vector<std::vector<Literal>> clauses
    );
    QuantifierType get_quantifier_type(VariableID) const;

    size_t num_variables;

    std::vector<QuantifierBlock> prefix;

    CNFMatrix matrix;

    std::unordered_map<
        VariableID,
        std::unordered_set<VariableID>
    > dependencies;

  private:
    std::vector<
      QuantifierType
        > var_quantifier_map_;

}; // class QBFFormula
