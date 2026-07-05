#pragma once

#include <memory>

#include "common/Result.hpp"

#include "formula/QBFFormula.hpp"
#include "formula/FormulaModifier.hpp"

class ResidualQBF {
public:
    explicit ResidualQBF(
        std::shared_ptr<const QBFFormula> formula
    );

    std::shared_ptr<const QBFFormula> formula;

    FormulaModifier modifier;

    bool is_cnf_empty() const;

    bool contains_empty_clause() const;

    bool contains_pure_clause() const;

    bool is_pure_existential() const;

    bool is_prefix_empty() const;

    SolverResult count_tautological_strategies() const;

    std::unique_ptr<QBFFormula>
      materialize() const;
};
