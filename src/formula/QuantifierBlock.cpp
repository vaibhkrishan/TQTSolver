#include "formula/QuantifierBlock.hpp"

QuantifierBlock::QuantifierBlock(QuantifierType type):
  type_(type) {
  }

QuantifierBlock::QuantifierBlock(QuantifierType type,
    std::vector<VariableID>& variables):
  type_(type),
  variables_(variables) {
  }

QuantifierType QuantifierBlock::get_type() const {
  return type_;
}

void QuantifierBlock::add_variable(VariableID var) {
  variables_.push_back(var);
}

std::vector<VariableID> QuantifierBlock::get_variables() const {
  return variables_;
}

bool QuantifierBlock::empty() const {
  return variables_.empty();
}

size_t QuantifierBlock::size() const {
  return variables_.size();
}
