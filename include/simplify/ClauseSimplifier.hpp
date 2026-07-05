#pragma once

#include "simplify/Simplifier.hpp"

class ClauseSimplifier : public Simplifier {
public:
    void apply(
        ResidualQBF& qbf
    ) override;
};
