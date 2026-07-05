#pragma once

#include "formula/ResidualQBF.hpp"
#include "common/Types.hpp"

namespace DeactivateUnusedExistentials {
    ExponentType apply(
        ResidualQBF& qbf
    );
}; // namespace DeactivateUnusedExistentials
