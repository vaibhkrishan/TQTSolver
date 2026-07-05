#include "simplify/DeactivateUnusedExistentials.hpp"

namespace DeactivateUnusedExistentials {
  ExponentType
    apply(
        ResidualQBF& qbf
        ) {

      ExponentType exponent = 0;

      ExponentType dependency_size = 0;

      for (
          size_t i = qbf.modifier.current_quantifier_block;
          i < qbf.formula->prefix.size();
          ++i
          ) {

        const auto& block =
          qbf.formula->prefix[i];

        if (
            block.get_type()
            == QuantifierType::FORALL
           ) {

          for (
              VariableID var
              : block.get_variables()
              ) {

            if (
                qbf.get_variable_status(var)
                == VariableStatus::ACTIVE
                or
                qbf.get_variable_status(var)
                == VariableStatus::UNUSED
               ) {

              ++dependency_size;
            }
          }

          continue;
        }

        for (
            VariableID var
            : block.get_variables()
            ) {

          if (
              qbf.get_variable_status(var)
              != VariableStatus::UNUSED
             ) {
            continue;
          }

          qbf.modifier.variable_status[var] =
            VariableStatus::INACTIVE;

          exponent +=
            ExponentType(1)
            << dependency_size;
        }
      }

      return exponent;
    }
}
