#pragma once

#include <vector>
#include "Random.h"
#include "functions.hpp"

class Chromosome
{
public:
    Chromosome() = default;
    Chromosome(const std::vector<double>& genes);
    Chromosome(std::vector<double>&& genes) noexcept;
    Chromosome(const Chromosome& other);
    Chromosome(Chromosome&& other) noexcept;
    ~Chromosome() = default;

    void                       evaluate_solution(TargetFunction fnc);
    void                       mutate(double mRate, double mStrength);
    void                       mutate_vm(double mRate, double mStrength);
    void                       checkBounds(TargetFunction target_fnc);
    double                     get_fitness() const { return m_fitness_value; }
    std::size_t                size() const { return m_chromosome.size(); }
    const std::vector<double>& get_genes_array() const { return m_chromosome; }

    bool operator<(const Chromosome& other) const;
    bool operator==(const Chromosome& other) const;
    Chromosome& operator=(const Chromosome& other);

    // friend void swap(Chromosome& first, Chromosome& second) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Chromosome& chromosome);
    
private:
    std::vector<double> m_chromosome{};
    double m_fitness_value{};

};
