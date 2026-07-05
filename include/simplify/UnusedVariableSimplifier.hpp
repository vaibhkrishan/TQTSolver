#pragma once

#include "simplify/Simplifier.hpp"

class UnusedVariableSimplifier : public Simplifier {
public:
    void apply(
        ResidualQBF& qbf
    ) override;
};
