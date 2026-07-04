#pragma once

#include <vector>
#include "common/Types.hpp"

class QuantifierBlock {
  public:
    QuantifierBlock(QuantifierType);
    QuantifierBlock(QuantifierType, std::vector<VariableID>&);

    QuantifierType get_type() const;

    void add_variable(VariableID);
    std::vector<VariableID> get_variables() const;

    bool empty() const;
    size_t size() const;

  private:
    QuantifierType type_;
    std::vector<VariableID> variables_;
};
