#pragma once

#include <vector>
#include <cmath>
#include <numeric>
#include <unordered_map>
#include <functional>
#include "constants.h"

// -------------------------------------------------------------------------------------------------------------------------------------

/// @brief  Rastrigin benchmark function
/// @param x parameters array
/// @return f(x...) 
inline double rastrigin_fnc(const std::vector<double>& x)
{
    constexpr double A{ 10 };
    double result{ A * x.size() };

    for(auto xi : x)
        result += (xi * xi - A * std::cos(2 * Constants::Math::pi * xi));
    
    return result;
}

// -------------------------------------------------------------------------------------------------------------------------------------

/// @brief  Ackley benchmark function
/// @param x parameters array
/// @return f(x...) 
inline double ackley_fnc(const std::vector<double>& v)
{
    constexpr double A{ 20 };
    constexpr double B{ 0.2 };
    constexpr double C{ 2 * Constants::Math::pi };
    const std::size_t nDim{ v.size() };

    double term1{ -A * std::exp(-B * std::sqrt(std::accumulate(v.begin(), v.end(), 0.0, [](double a, double b) { return a + b * b; }) / nDim)) };

    double sum_cos = 0.0;
    for (const auto& x: v) 
        sum_cos += std::cos(C * x);
    
    double term2{ -std::exp(sum_cos / nDim) };

    return term1 + term2 + std::exp(1) + A;
}

// -------------------------------------------------------------------------------------------------------------------------------------

/// @brief  Sphere benchmark function
/// @param x parameters array
/// @return f(x...) 
inline double sphere_fnc(const std::vector<double>& x)
{
    double result{ 0.0 };

    for(auto xi : x)
        result += xi * xi;

    return result;    
}

// -------------------------------------------------------------------------------------------------------------------------------------

/// @brief  Easom benchmark function
/// @param x parameters array
/// @return f(x...) 
inline double easom_fnc(const std::vector<double>& v)
{
    const auto x{ v[0] };
    const auto y{ v[1] };

    return -std::cos(x) * std::cos(y) * std::exp(-((x - Constants::Math::pi) * (x - Constants::Math::pi)
        + (y - Constants::Math::pi) * (y - Constants::Math::pi)));
}

// -------------------------------------------------------------------------------------------------------------------------------------

/// @brief  McCormick benchmark function
/// @param x parameters array
/// @return f(x...) 
inline double mccormick_fnc(const std::vector<double>& v)
{
    const auto x{ v[0] };
    const auto y{ v[1] };

    return std::sin(x + y) + (x - y) * (x - y) - 1.5*x + 2.5*y + 1;
}

// -------------------------------------------------------------------------------------------------------------------------------------

namespace Benchmark {
    using FncPtr = std::function<double(const std::vector<double>&)>;
    using enum TargetFunction;
    
    inline std::unordered_map<TargetFunction, FncPtr> target_functions {
        {rastrigin, rastrigin_fnc},
        {ackley, ackley_fnc},
        {sphere, sphere_fnc},
        {easom, easom_fnc},
        {mccormick, mccormick_fnc}
    };
}
