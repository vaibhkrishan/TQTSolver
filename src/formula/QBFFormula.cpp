#include "formula/QBFFormula.hpp"

QuantifierType QBFFormula::get_quantifier_type(VariableID var) const {
  auto it = var_quantifier_map.find(var);
  if (it != var_quantifier_map.end())
    return it->second;

  return QuantifierType::EXISTS;
}

void QBFFormula::set_quantifier_type(VariableID var, QuantifierType quantifierType) {
  var_quantifier_map[var] = quantifierType;
}

bool
QBFFormula::contains_pure_clause() const {

  for (
      const auto& clause :
      matrix.clauses
      ) {

    bool pure = true;

    for (
        Literal lit :
        clause
        ) {

      VariableID variable =
        std::abs(lit);

      auto it =
        var_quantifier_map
        .find(variable);

      if (
          it ==
          var_quantifier_map
          .end()
         ) {

        throw std::runtime_error(
            "Unknown variable in clause."
            );
      }

      if (
          it->second ==
          QuantifierType::EXISTS
         ) {

        pure = false;
        break;
      }
    }

    if (
        pure
       ) {

      return true;
    }
  }

  return false;
}

bool
QBFFormula::is_pure_existential() const {

  for (
      const auto& clause :
      matrix.clauses
      ) {

    for (
        Literal lit :
        clause
        ) {

      VariableID variable =
        std::abs(lit);

      auto it =
        var_quantifier_map.find(
            variable
            );

      if (
          it !=
          var_quantifier_map.end()
          &&
          it->second ==
          QuantifierType::FORALL
         ) {

        return false;
      }
    }
  }

  return true;
}
