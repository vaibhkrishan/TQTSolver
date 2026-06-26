#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "QuantifierBlock.hpp"
#include "CNFMatrix.hpp"

class QBFFormula {
  public:
    int num_variables;

    std::vector<QuantifierBlock> prefix;
    CNFMatrix matrix;
    std::unordered_map< VariableID, std::unordered_set<VariableID> > dependencies;
    QuantifierType get_quantifier_type(VariableID) const;
    void set_quantifier_type(VariableID, QuantifierType);
    bool contains_pure_clause() const;
    bool is_pure_existential() const;

  private:
    std::unordered_map<
      VariableID,
      QuantifierType
        > var_quantifier_map;

}; // class QBFFormula
