#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "QuantifierBlock.hpp"
#include "CNFMatrix.hpp"

class QBFFormula {
public:
  std::vector<QuantifierBlock> prefix;
  CNFMatrix matrix;
  std::unordered_map< VariableID, std::unordered_set<VariableID> > dependencies;
};
