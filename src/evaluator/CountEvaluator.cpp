#include "evaluator/CountEvaluator.hpp"

CountEvaluator::CountEvaluator(
    const SearchState& state,
    const QBFFormula& formula
    )
  : state_(state),
  formula_(formula)
{
  for (
      const auto& edge :
      state_.edges
      ) {

    children_[edge.parent]
      .push_back(
          edge.child
          );

    branch_variable_
      .try_emplace(
          edge.parent,
          edge.variable
          );
  }
}

SolverResult
CountEvaluator::evaluate() {

  return evaluate_node(
      root_id
      );
}

bool
CountEvaluator::is_existential(
    VariableID variable
    ) const {

  return
    formula_
    .get_quantifier_type(variable)
    ==
    QuantifierType::EXISTS;
}

SolverResult
CountEvaluator::evaluate_node(
    NodeID id
    ) {

  auto memo =
    memo_.find(
        id
        );

  if (
      memo !=
      memo_.end()
     ) {

    return memo->second;
  }

  const auto& node =
    state_
    .nodes
    .at(id);

  SolverResult result;

  switch (
      node.status
      ) {

    case NodeStatus::LEAF_TAUT:

      result =
        node.value;

      break;

    case NodeStatus::LEAF_FQBF:

      result =
        node.value;

      break;

    case NodeStatus::SEEN:
      {

        const auto& equivalent =
          state_
          .nodes
          .at(
              node.equivalent_node
             );

        result =
          evaluate_node(
              node.equivalent_node
              );

        auto delta =
          node.multiplier_exponent
          -
          equivalent.multiplier_exponent;

        if (
            delta > 0
           ) {

          result <<=
            delta;
        }
        else if (
            delta < 0
            ) {

          result >>=
            -delta;
        }

        break;
      }

    case NodeStatus::EXPANDED:
      {

        VariableID variable =
          branch_variable_
          .at(id);

        if (
            is_existential(
              variable
              )
           ) {

          result = 0;

          for (
              NodeID child :
              children_.at(id)
              ) {

            result +=
              evaluate_node(
                  child
                  );
          }
        }
        else {

          result = 1;

          for (
              NodeID child :
              children_.at(id)
              ) {

            result *=
              evaluate_node(
                  child
                  );
          }
        }

        break;
      }

    default:
      std::cerr << "Unknown node status, node_id: " << node.id << std::endl;
  }

  memo_[id] =
    result;

  return result;
}
