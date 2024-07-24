#include "Chromosome.h"
#include "Utils.h"

// -------------------------------------------------------------------------------------------------------------------------------------

Chromosome::Chromosome(const std::vector<double>& genes)
    : m_chromosome{ genes }, m_fitness_value{ 10000.0 } 
    {
    }

Chromosome::Chromosome(std::vector<double>&& genes) noexcept
        : m_chromosome{ std::move(genes) }, m_fitness_value{ 10000.0 }
    {
    }

Chromosome::Chromosome(const Chromosome& other)
    : m_chromosome{ other.m_chromosome }, m_fitness_value{ other.m_fitness_value }
    {
    }

Chromosome::Chromosome(Chromosome&& other) noexcept 
    : m_chromosome{ std::move(other.m_chromosome) }, m_fitness_value{ other.m_fitness_value }
    {
    }

// -------------------------------------------------------------------------------------------------------------------------------------

void Chromosome::evaluate_solution(TargetFunction fnc)
{
    m_fitness_value = Benchmark::target_functions[fnc](m_chromosome);
}

// -------------------------------------------------------------------------------------------------------------------------------------

void Chromosome::mutate(double mRate, double mStrength)
{
    std::normal_distribution<> dist(0, mStrength);

    for(auto& gene: m_chromosome)
        if(Random::rand() < mRate)
            gene += dist(Random::mt);
}

// -------------------------------------------------------------------------------------------------------------------------------------

void Chromosome::mutate_vm(double mRate, double mStrength)
{
    std::normal_distribution<> dist(0, mStrength);

    for(auto& gene: m_chromosome)
    {
        if(Random::rand() < mRate)
        {
            if(Random::rand() < 1.0e-4)
                gene *= dist(Random::mt);
            else
                gene += dist(Random::mt);    
        }
    }
}

// -------------------------------------------------------------------------------------------------------------------------------------

void Chromosome::checkBounds(TargetFunction fnc) 
{
    auto bounds{ getBound(fnc) };
    
    if (fnc == TargetFunction::mccormick) 
    {
        auto bounds_pair{ std::get<BoundsPair>(bounds) };

        auto [x_lower, x_upper]{ std::get<static_cast<int>(BoundType::lower)>(bounds_pair) };
        auto [y_lower, y_upper]{ std::get<static_cast<int>(BoundType::higher)>(bounds_pair) };

        for (std::size_t i {0}; i < m_chromosome.size(); ++i) 
        {
            if(i == 0) 
                m_chromosome[i] = std::clamp(m_chromosome[i], x_lower, x_upper);
            else if(i == 1)
                m_chromosome[i] = std::clamp(m_chromosome[i], y_lower, y_upper);
        }
    } 
    else 
    {
        auto bounds_single{ std::get<Bounds>(bounds) };
        auto [lower, upper]{ bounds_single };

        for (auto& gene : m_chromosome) 
            gene = std::clamp(gene, lower, upper);
    }
}

// -------------------------------------------------------------------------------------------------------------------------------------

bool Chromosome::operator<(const Chromosome& other) const 
{
    return m_fitness_value < other.m_fitness_value;
}

bool Chromosome::operator==(const Chromosome& other) const 
{
    return m_chromosome == other.m_chromosome;
}

Chromosome& Chromosome::operator=(const Chromosome& other) 
{
    if(this != &other) 
    {
        m_chromosome = other.m_chromosome;
        m_fitness_value = other.m_fitness_value;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Chromosome& chromosome) 
{
    os << "[";
    for (std::size_t i {0}; i < chromosome.m_chromosome.size(); ++i) 
    {
        os << chromosome.m_chromosome[i];
        if(i != chromosome.m_chromosome.size() - 1) 
            os << ", ";
    }

    os << "]";
    
    return os;
}

// void swap(Chromosome& first, Chromosome& second) noexcept {
//     using std::swap;
//     swap(first.m_chromosome, second.m_chromosome);
//     swap(first.m_fitness_value, second.m_fitness_value);
// }
