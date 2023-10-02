#pragma once
#include <concepts>

inline namespace pi {

template<typename Numeric, std::floating_point Real>
requires std::is_arithmetic_v<Numeric>
Real divide_by(Numeric numerator, Real denominator)
{
    return static_cast<Real>(numerator)/denominator;
}
}