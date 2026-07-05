#pragma once

#include <vector>

#include "common/Types.hpp"

class CNFMatrix {
public:
    CNFMatrix() = default;

    explicit CNFMatrix(
        std::vector<std::vector<Literal>> clauses
    );

    std::vector<std::vector<Literal>> clauses;
    const std::vector<std::vector<Literal>>& get_clauses() const;
};
