#include "evaluator/CountEvaluator.hpp"

#include <queue>
#include <unordered_map>
#include <vector>
#include <stdexcept>

#include "explorer/NodeStatus.hpp"

namespace {

  bool is_leaf(
      NodeStatus status
      ) {

    switch (status) {

      case NodeStatus::SEEN:
      case NodeStatus::LEAF_TAUT:
      case NodeStatus::LEAF_FQBF:
        return true;

      default:
        return false;
    }
  }

  bool is_existential(
      const QBFFormula& formula,
      VariableID variable
      ) {

    for (
        const auto& block
        : formula.prefix
        ) {

      for (
          VariableID v
          : block.variables
          ) {

        if (
            v == variable
           ) {

          return block.type
            == QuantifierType::EXISTS;
        }
      }
    }

    throw std::runtime_error(
        "Variable not found in quantifier prefix"
        );
  }

} // namespace

SolverResult CountEvaluator::evaluate(
    const SearchState& state,
    const QBFFormula& formula
    ) {

  std::unordered_map<
    NodeID,
  std::vector<SearchEdge>
    > outgoing;

  std::unordered_map<
    NodeID,
    std::vector<NodeID>
      > parents;

  std::unordered_map<
    NodeID,
    std::size_t
      > remaining_children;

  for (
      const auto& [id, node]
      : state.nodes
      ) {

    remaining_children[id] = root_id;
  }

  for (
      const auto& edge
      : state.edges
      ) {

    outgoing[
      edge.parent
    ].push_back(
        edge
        );

    parents[
      edge.child
    ].push_back(
        edge.parent
        );

    remaining_children[
      edge.parent
    ]++;
  }

  std::queue<NodeID> ready;

  std::unordered_map<
    NodeID,
    SolverResult
      > value;

  for (
      const auto& [id, node]
      : state.nodes
      ) {

    if (
        remaining_children[id]
        == root_id
       ) {

      ready.push(id);

      value[id] =
        node.value;
    }
  }

  while (
      !ready.empty()
      ) {

    NodeID node_id =
      ready.front();

    ready.pop();

    auto parent_it =
      parents.find(
          node_id
          );

    if (
        parent_it
        == parents.end()
       ) {

      continue;
    }

    for (
        NodeID parent_id
        : parent_it->second
        ) {

      remaining_children[
        parent_id
      ]--;

      if (
          remaining_children[
          parent_id
          ]
          != root_id
         ) {

        continue;
      }

      const auto& children =
        outgoing.at(
            parent_id
            );

      if (
          children.empty()
         ) {

        value[parent_id] =
          state.nodes
          .at(parent_id)
          .value;

        ready.push(
            parent_id
            );

        continue;
      }

      VariableID variable =
        children.front()
        .variable;

      bool existential =
            formula.get_quantifier_type(variable) == QuantifierType::EXISTS;

      SolverResult result;

      if (
          existential
         ) {

        result = 0;

        for (
            const auto& edge
            : children
            ) {

          result +=
            value.at(
                edge.child
                );
        }
      }
      else {

        result = 1;

        for (
            const auto& edge
            : children
            ) {

          result *=
            value.at(
                edge.child
                );
        }
      }

      value[parent_id] =
        result;

      ready.push(
          parent_id
          );
    }
  }

  auto root_it =
    state.nodes.find(root_id);

  if (
      root_it
      == state.nodes.end()
     ) {

    throw std::runtime_error(
        "Root node missing"
        );
  }

  return value.at(root_id);
}
