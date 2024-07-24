#pragma once

#include <utility>
#include <vector>
#include <string_view>
#include <array>
#include <unordered_map>
#include <tuple>
#include <variant>
#include <stdexcept>

// -------------------------------------------------------------------------------------------------------------------------------------

using namespace std::string_view_literals;
using SingleBound = double;
using PairBound = std::pair<double, double>;
using Bounds = std::tuple<SingleBound, SingleBound>;
using BoundsPair = std::tuple<PairBound, PairBound>;
#define BEST_SOLUTION 0

// -------------------------------------------------------------------------------------------------------------------------------------

// Enum para representar as funções alvo
enum class TargetFunction {
    rastrigin,
    ackley,
    sphere,
    easom,
    mccormick,

    max_functions
};

// -------------------------------------------------------------------------------------------------------------------------------------

namespace Constants {

    namespace Math {
        inline constexpr double pi{ 3.14159265358979323846 };
    }

    inline constexpr std::tuple<Bounds, Bounds, Bounds, Bounds, BoundsPair> BOUNDS {
        std::make_tuple (
            Bounds{-5.12, 5.12},
            Bounds{-5.0, 5.0},
            Bounds{-1000.0, 1000.0},
            Bounds{-100.0, 100.0},
            BoundsPair{ {-1.5, 4.0}, {-3.0, 4.0} }
        )
    };

    constexpr std::array<std::string_view, static_cast<size_t>(TargetFunction::max_functions)> functionNames {
        "Rastrigin"sv, "Ackley"sv, "Sphere"sv, "Easom"sv, "McCormick"sv
    };

}

// -------------------------------------------------------------------------------------------------------------------------------------

// Enum para representar os limites inferior e superior
enum class BoundType {
    lower,
    higher
};

// -------------------------------------------------------------------------------------------------------------------------------------

// Enum para representar os métodos de seleção
enum class SelectionMethod {
    tournament,
    fps, // Fitness Proportionate Selection
    ranking
};

// -------------------------------------------------------------------------------------------------------------------------------------

// Enum para representar o número de pontos do crossover
enum class Points {
    one,
    two,
    uniform
};

// -------------------------------------------------------------------------------------------------------------------------------------

// Sobrecarga do operador << para imprimir Bounds
inline std::ostream& operator<<(std::ostream& os, const Bounds& bounds) {
    using enum BoundType;
    os << "(" << std::get<static_cast<std::size_t>(lower)>(bounds) << ", " << std::get<static_cast<std::size_t>(higher)>(bounds) << ")";
    return os;
}

// -------------------------------------------------------------------------------------------------------------------------------------

// Sobrecarga do operador << para imprimir BoundsPair
inline std::ostream& operator<<(std::ostream& os, const BoundsPair& boundsPair) {
    using enum BoundType;
    os << "(" << std::get<static_cast<std::size_t>(lower)>(boundsPair).first << ", " << std::get<static_cast<std::size_t>(lower)>(boundsPair).second << ")"
    << " (" << std::get<static_cast<std::size_t>(higher)>(boundsPair).first << ", " << std::get<static_cast<std::size_t>(higher)>(boundsPair).second << ")";
    return os;
}

// -------------------------------------------------------------------------------------------------------------------------------------

// Função para obter o nome da função alvo
constexpr std::string_view getFunctionName(TargetFunction function) {
    return Constants::functionNames[static_cast<std::size_t>(function)];
}

// -------------------------------------------------------------------------------------------------------------------------------------

// Sobrecarga do operador << para TargetFunction
inline std::ostream& operator<<(std::ostream& os, TargetFunction function) {
    return os << getFunctionName(function);
}

// -------------------------------------------------------------------------------------------------------------------------------------

// Funções auxiliares para obter limites
template <TargetFunction F, BoundType B>
constexpr auto getBound() {
    using enum TargetFunction;
    using enum BoundType;
    if constexpr (F == TargetFunction::mccormick) 
    {
        if constexpr (B == BoundType::lower) 
            return std::get<static_cast<std::size_t>(lower)>(std::get<static_cast<std::size_t>(mccormick)>(Constants::BOUNDS));
        else 
            return std::get<static_cast<std::size_t>(higher)>(std::get<static_cast<std::size_t>(mccormick)>(Constants::BOUNDS));
    } 
    else 
        return std::get<static_cast<std::size_t>(B)>(std::get<static_cast<std::size_t>(F)>(Constants::BOUNDS));
}

// -------------------------------------------------------------------------------------------------------------------------------------

using BoundVariant = std::variant<Bounds, BoundsPair>;
inline BoundVariant getBound(TargetFunction f) {
    using enum TargetFunction;
    switch (f) {
        case TargetFunction::rastrigin:
            return std::get<static_cast<int>(rastrigin)>(Constants::BOUNDS);
        case TargetFunction::ackley:
            return std::get<static_cast<int>(ackley)>(Constants::BOUNDS);
        case TargetFunction::sphere:
            return std::get<static_cast<int>(sphere)>(Constants::BOUNDS);
        case TargetFunction::easom:
            return std::get<static_cast<int>(easom)>(Constants::BOUNDS);
        case TargetFunction::mccormick:
            return std::get<static_cast<int>(mccormick)>(Constants::BOUNDS);
        default:
            throw std::invalid_argument("Invalid target function");
    }
}
