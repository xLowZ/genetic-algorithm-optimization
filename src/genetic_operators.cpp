#include <algorithm>
#include <omp.h>
#include "Utils.h"
#include "genetic_operators.h"

// -------------------------------------------------------------------------------------------------------------------------------------

// std::vector<int> selectRandomIndices(int populationSize, int numCandidates);
std::vector<double> getFitnessArray(const std::vector<Chromosome>& pop);

// -------------------------------------------------------------------------------------------------------------------------------------

std::vector<Chromosome> initialization(TargetFunction target_fnc, int dimensions, int populationSize)
{
    if(populationSize <= 0 || dimensions <= 0) 
        throw std::invalid_argument("Invalid parameters provided.");
   
    std::vector<Chromosome> initial_population;
    initial_population.reserve(populationSize);

   using enum TargetFunction;
   using enum BoundType;
   using enum Points;
   using enum SelectionMethod;

   auto bounds{ getBound(target_fnc) };

   if(target_fnc == mccormick)
   {
        auto bounds_pair{ std::get<BoundsPair>(bounds) }; // Lida com BoundPair

        auto [x_lower, x_upper]{ std::get<static_cast<int>(lower)>(bounds_pair) };
        auto [y_lower, y_upper]{ std::get<static_cast<int>(higher)>(bounds_pair) };

       
        for(int i {0}; i < populationSize; ++i)
        {
            double x{ Random::get(x_lower, x_upper) };
            double y{ Random::get(y_lower, y_upper) };

            initial_population.emplace_back(std::vector<double>{ x, y });
        }
   }      
   else
   {
        auto bounds_single = std::get<Bounds>(bounds); // Acessa o Bounds específico
        auto [lower, upper] = bounds_single;

        std::vector<double> genes(dimensions);
        
        for(int i {0}; i < populationSize; ++i)
        {
            for (auto& gene : genes) 
                gene = Random::get(lower, upper);
            
            initial_population.emplace_back(genes);
        }
   }

   return initial_population;
}

// -------------------------------------------------------------------------------------------------------------------------------------

void evaluatePopulation(std::vector<Chromosome>& population, TargetFunction target_fnc)
{
    for(Chromosome& individual : population)
        individual.evaluate_solution(target_fnc);
}

// -------------------------------------------------------------------------------------------------------------------------------------

const Chromosome& selection(const std::vector<Chromosome>& population, int populationSize, SelectionMethod method, int numCandidates)
{
    if(method == SelectionMethod::tournament)
    {
        std::vector<int> selectedIndices(numCandidates);
        for (int i = 0; i < numCandidates; ++i) 
            selectedIndices[i] = Random::uniform(0, populationSize);

        // Encontra o índice do vencedor com base no menor fitness
        int winnerIndex { selectedIndices[0] }; 

        for (int i = 1 ; i < numCandidates; ++i) 
        {
            if(population[selectedIndices[i]].get_fitness() < population[winnerIndex].get_fitness()) 
                winnerIndex = selectedIndices[i];
        }
        
        return population[winnerIndex];
    }

    else if(method == SelectionMethod::fps)
    {
        std::vector<double> fitnessArray{ getFitnessArray(population) };

        double maxFitness{ *std::max_element(fitnessArray.begin(), fitnessArray.end()) };
        std::transform(fitnessArray.begin(), fitnessArray.end(), fitnessArray.begin(),
                [maxFitness](double f) { return maxFitness - f + 1e-6; });

        double totalFitness{ std::accumulate(fitnessArray.begin(), fitnessArray.end(), 0.0) };

        std::vector<double> probabilities(populationSize);

        std::transform(fitnessArray.begin(), fitnessArray.end(), probabilities.begin(),
                [totalFitness](double f) { return f / totalFitness; });
        
        std::vector<double> roulette(populationSize);
        std::partial_sum(probabilities.begin(), probabilities.end(), std::back_inserter(roulette));

        double magicNum{ Random::rand() };
        auto it{ std::lower_bound(roulette.begin(), roulette.end(), magicNum) };
        return population[std::distance(roulette.begin(), it)];
    }

    else if(method == SelectionMethod::ranking) 
    {
        constexpr double min{ 0.8 };
        constexpr double max{ 1.1 };

        // Calcular as probabilidades de seleção com base nos indices
        std::vector<double> probabilities(populationSize);
        for (int i {0}; i < populationSize; ++i)
            probabilities[i] = max - (max - min) * (static_cast<double>(i) / (populationSize - 1));
    
        // Normalizar as probabilidades para garantir que a soma seja 1
        double totalProbability{ std::accumulate(probabilities.begin(), probabilities.end(), 0.0) };
        for(double& prob : probabilities) 
            prob /= totalProbability;

        // Criar uma distribuição acumulada dessas probabilidades
        std::vector<double> cumulativeProb(populationSize);
        std::partial_sum(probabilities.begin(), probabilities.end(), cumulativeProb.begin());

        double magicNum{ Random::rand() };
        auto it{ std::lower_bound(cumulativeProb.begin(), cumulativeProb.end(), magicNum) };

        if(it == cumulativeProb.end()) // Caso de arredondamento
            return population.back();

        return population[std::distance(cumulativeProb.begin(), it)];
    }

    return * std::min_element(population.begin(), population.end(),
                [](const Chromosome& a, const Chromosome& b) { return a.get_fitness() < b.get_fitness(); });
}

// -------------------------------------------------------------------------------------------------------------------------------------

std::pair<Chromosome, Chromosome> crossover(const Chromosome& parent1, const Chromosome& parent2, Points nPoints)
{
    const std::vector<double>& firstParentGenes { parent1.get_genes_array() };
    const std::vector<double>& secondParentGenes{ parent2.get_genes_array() };

    int size{ static_cast<int>(parent1.size()) };

    std::vector<double> firstChildGenes (size);
    std::vector<double> secondChildGenes(size);

    auto firstParentBegin{ firstParentGenes.begin() }; 
    auto firstParentEnd{ firstParentGenes.end() }; 

    auto secondParentBegin{ secondParentGenes.begin() }; 
    auto secondParentEnd{ secondParentGenes.end() }; 

    auto firstChildIt{ firstChildGenes.begin() }; 
    auto secondChildIt{ secondChildGenes.begin() };

    if(nPoints == Points::one || size == 2)
    {
        int point{ Random::uniform(1, size) };

        std::copy(firstParentBegin, firstParentBegin + point, firstChildIt);
        std::copy(secondParentBegin + point, secondParentEnd, firstChildIt + point);

        std::copy(secondParentBegin, secondParentBegin + point, secondChildIt);
        std::copy(firstParentBegin + point, firstParentEnd, secondChildIt + point);
    }

    else if(nPoints == Points::two)
    {
        std::pair<int, int> points{ Random::uniform(1, size), Random::uniform(1, size) };

        // Garante que os pontos são únicos e ordenados
        while(points.first == points.second) 
            points.second = Random::uniform(1, size);
        
        if (points.first > points.second)
            std::swap(points.first, points.second);

        int point1{ points.first };
        int point2{ points.second };

        std::copy(firstParentBegin, firstParentBegin + point1, firstChildIt);
        std::copy(secondParentBegin + point1, secondParentBegin + point2, firstChildIt + point1);
        std::copy(firstParentBegin + point2, firstParentEnd, firstChildIt + point2);

        std::copy(secondParentBegin, secondParentBegin + point1, secondChildIt);
        std::copy(firstParentBegin + point1, firstParentBegin + point2, secondChildIt + point1);
        std::copy(secondParentBegin + point2, secondParentEnd, secondChildIt + point2);
    }
    
    else if(nPoints == Points::uniform)
    {
        std::vector<int> mask(size);

        for(int i {0}; i < size; ++i)
            mask[i] = Random::get(0, 1);

        for(int i {0}; i < size; i++)
        {
            if(mask[i] == 0)
            {
                firstChildGenes[i] = firstParentGenes[i];
                secondChildGenes[i] = secondParentGenes[i];
            }
            else
            {
                firstChildGenes[i] = secondParentGenes[i];
                secondChildGenes[i] = firstParentGenes[i];
            }
        }    
    }

    return { Chromosome(firstChildGenes), Chromosome(secondChildGenes) };
}

// -------------------------------------------------------------------------------------------------------------------------------------

void mutation(Chromosome& child1, Chromosome& child2, const Parameters& p)
{
    Chromosome copy1{ child1 };
    Chromosome copy2{ child2 };

    copy1.mutate(p.mutation_rate, p.mutation_strength);
    copy2.mutate(p.mutation_rate, p.mutation_strength);

    copy1.checkBounds(p.target_function);
    copy2.checkBounds(p.target_function);

    copy1.evaluate_solution(p.target_function);
    copy2.evaluate_solution(p.target_function);

    if(copy1.get_fitness() < child1.get_fitness())
        child1 = std::move(copy1);

    if(copy2.get_fitness() < child2.get_fitness())
        child2 = std::move(copy2);
}

// -------------------------------------------------------------------------------------------------------------------------------------

/*void mutation(Chromosome& child, const Parameters& p)
{
    Chromosome copy{ child };
    copy.mutate(p.mutation_rate, p.mutation_strength);
    copy.checkBounds(p.target_function);
    copy.evaluate_solution(p.target_function);
    if(copy.get_fitness() < child.get_fitness())
        child = std::move(copy);
}*/

// -------------------------------------------------------------------------------------------------------------------------------------

std::vector<Chromosome> createNewGeneration(const std::vector<Chromosome>& prev_gen, const Parameters& p)
{
    int numElites{ std::max(1, static_cast<int>(p.elite_fraction * p.pop_size)) };

    std::vector<Chromosome> newGeneration;
    newGeneration.reserve(p.pop_size);

    for(int i {0}; i < numElites; ++i) 
        newGeneration.push_back(prev_gen[i]);

    if(prev_gen[0].size() > 1)
    {
        while(std::ssize(newGeneration) < p.pop_size)
        {
            const Chromosome& firstParent { selection(prev_gen, p.pop_size, p.method) };
            const Chromosome& secondParent{ selection(prev_gen, p.pop_size, p.method) };

            auto [firstChild, secondChild]{ crossover(firstParent, secondParent, p.points) };

            firstChild.evaluate_solution(p.target_function);
            secondChild.evaluate_solution(p.target_function);

            mutation(firstChild, secondChild, p);

            newGeneration.push_back(firstChild);
            newGeneration.push_back(secondChild);
        }
    } 
    else
    {
        while(std::ssize(newGeneration) < p.pop_size)
        {
            const Chromosome& parent{ selection(prev_gen, p.pop_size, p.method) };
            Chromosome child{ parent };

            child.mutate(p.mutation_rate, p.mutation_strength);

            child.checkBounds(p.target_function);

            child.evaluate_solution(p.target_function);

            if(child.get_fitness() < parent.get_fitness()) 
                newGeneration.push_back(child);
            else 
                newGeneration.push_back(parent);
        }
    }   

    std::sort(newGeneration.begin(), newGeneration.end());

    return newGeneration;
}

// -------------------------------------------------------------------------------------------------------------------------------------

std::vector<Chromosome> parallelCreateNewGeneration(const std::vector<Chromosome>& prev_gen, const Parameters& p, int numThreads)
{
    int numElites{ std::max(1, static_cast<int>(p.elite_fraction * p.pop_size)) };

    (numElites & 1) ? numElites &= ~1 : numElites;
    
    int numNew{ p.pop_size - numElites };

    std::vector<Chromosome> newGeneration;
    newGeneration.reserve(p.pop_size);

    for(int i {0}; i < numElites; ++i) 
        newGeneration.push_back(prev_gen[i]);

    std::vector<Chromosome> offspring(numNew);

    if(prev_gen[0].size() > 1)
    {
        #pragma omp parallel for schedule(static) num_threads(numThreads)
        for(int i = 0; i < numNew / 2; ++i)
        {
            const Chromosome& firstParent  { selection(prev_gen, p.pop_size, p.method) };
            const Chromosome& secondParent { selection(prev_gen, p.pop_size, p.method) };

            auto [firstChild, secondChild]{ crossover(firstParent, secondParent, p.points) };

            mutation(firstChild, secondChild, p);

            int idx{ i * 2 };
            offspring[idx] = firstChild;
            offspring[idx + 1] = secondChild;
        }
    }
    else
    {
        #pragma omp parallel for schedule(static) num_threads(numThreads)
        for(int i = 0; i < numNew; ++i)
        {
            const Chromosome& parent{ selection(prev_gen, p.pop_size, p.method) };
            Chromosome child{ parent };

            child.mutate(p.mutation_rate, p.mutation_strength);

            child.checkBounds(p.target_function);

            child.evaluate_solution(p.target_function);

            offspring[i] = (child.get_fitness() < parent.get_fitness()) ? child : parent;
        }
    }

    std::move(offspring.begin(), offspring.end(), std::back_inserter(newGeneration));
    std::sort(newGeneration.begin(), newGeneration.end());

    return newGeneration;
}

// -----------------------------------------------------------------------------------------------------------------------------------------------

// std::vector<int> selectRandomIndices(int populationSize, int numCandidates) 
// {
//     std::vector<int> indices(populationSize);

//     std::iota(indices.begin(), indices.end(), 0);

//     std::shuffle(indices.begin(), indices.end(), Random::mt);
    
//     return std::vector<int>(indices.begin(), indices.begin() + numCandidates);
// }

std::vector<double> getFitnessArray(const std::vector<Chromosome>& pop)
{
    std::vector<double> fitnessArray(pop.size());

    for(std::size_t i {0}; i < pop.size(); ++i)
        fitnessArray[i] = pop[i].get_fitness();

    return fitnessArray;
}
