#include "formula/QBFFormula.hpp"

QBFFormula::QBFFormula(
    size_t num_variables,
    std::vector<QuantifierBlock> prefix,
    std::vector<std::vector<Literal>> clauses
)
    : num_variables(num_variables),
      prefix(std::move(prefix)),
      matrix(std::move(clauses)),
      var_quantifier_map_(
          num_variables + 1,
          QuantifierType::EXISTS
      )
{
    for (const auto& block : this->prefix) {
        for (VariableID var : block.get_variables()) {
            var_quantifier_map_[var] = block.get_type();
        }
    }
}

QuantifierType QBFFormula::get_quantifier_type(VariableID var) const {
  return var_quantifier_map_.at(var);
}
