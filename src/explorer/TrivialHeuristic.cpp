#include "explorer/TrivialHeuristic.hpp"

VariableID
TrivialHeuristic::choose_variable(
    const ResidualQBF& qbf
) {

    const QBFFormula& formula =
        qbf.materialize();

    for (const auto& block :
         formula.prefix) {

        for (VariableID var :
             block.variables) {

            return var;
        }
    }

    return -1;
}
