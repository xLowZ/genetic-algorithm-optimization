#include <iostream>
#include <vector>
#include <omp.h>
#include "constants.h"
#include "Utils.h"
#include "Chromosome.h"
#include "Parameters.h"
#include "Timer.h"
#include "genetic_operators.h"
#include "FileLoader.h"

// -------------------------------------------------------------------------------------------------------------------------------------

void printSolution(const Chromosome& solution, int generation);
void printResults(std::vector<Chromosome>& solutions, const Parameters& p);
void adjustParallelPopulation(Parameters& p);
Chromosome geneticAlgorithm(Parameters& p, int numThreads, bool parallel);

// -------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
   Parameters params{};
   if(argc > 1) 
   {
      std::string arg1{ argv[1] };

      if (arg1 == "--help") 
      {
         print_help();
         std::cin.get();
         return 0;
      } 
      else 
      {
         std::string configFilePath{ arg1 };
         std::replace(configFilePath.begin(), configFilePath.end(), '/', '\\');
         params = FileLoader::loadFromTXT(configFilePath);
      }
   } 
   else 
   {
      printNoArgs();
      std::cin.get();
      return 0;
   }

   Settings::setup_precision(params.print_precision);
   const int maxThreads{ Settings::MultiThread::maxThreads };
   const bool populationParallelThreshold{ (params.pop_size >= Settings::MultiThread::POPULATION_PARALLEL_THRESHOLD) || (params.dimensions >= 20) };
   /**/bool sphereBug{ populationParallelThreshold && (params.target_function == TargetFunction::sphere) && params.dimensions > 3 };/**/
   omp_set_nested(1);  
   omp_set_num_threads(maxThreads);

   if(populationParallelThreshold && (params.pop_size & 1))
      adjustParallelPopulation(params);

   std::vector<Chromosome> topSolutions(params.num_tests);

   int remainingTests{ params.num_tests };
   Timer t;
   #pragma omp parallel for schedule(static) num_threads(maxThreads)
   for(int i = 0; i < params.num_tests; ++i)
   {
      bool should_parallelize { 
         (populationParallelThreshold) &&
         (maxThreads % remainingTests == 0) && 
         (remainingTests < maxThreads) && 
         !(sphereBug) 
      };

      int numThreads{ (remainingTests==1) ? maxThreads : maxThreads / 2 }; 

      Chromosome bestSolution{ geneticAlgorithm(params, numThreads, should_parallelize) };
      topSolutions[i] = bestSolution;

      #pragma omp critical
      {
         --remainingTests;
      }
   }

   auto time{ t.elapsed() };

   printResults(topSolutions, params);

   printElapsedTime(time);

   std::cin.get();
   return 0;
}

// -------------------------------------------------------------------------------------------------------------------------------------

Chromosome geneticAlgorithm(Parameters& p, int numThreads, bool parallel)
{
   std::vector<Chromosome> population{ initialization(p.target_function, p.dimensions, p.pop_size) };
   
   evaluatePopulation(population, p.target_function);

   std::sort(population.begin(), population.end());

   for(int generation {0}; generation < p.nIterations; ++generation)
   {
      auto linearDecay = [generation, &p](double initial_rate, double final_rate) {
         return initial_rate - (static_cast<double>(generation) / p.nIterations) * (initial_rate - final_rate);
      };

      p.mutation_rate = linearDecay(p.initial_mutation_rate, p.final_mutation_rate);
      p.mutation_strength = linearDecay(p.initial_mutation_strength, p.final_mutation_strength);

      if(parallel)
         population = parallelCreateNewGeneration(population, p, numThreads);
      else
         population = createNewGeneration(population, p);
        
      // Imprimir a cada 100 gerações
      if((generation + 1) % 100 == 0 || generation == p.nIterations - 1)
         printSolution(population[BEST_SOLUTION], generation);
   }

   return population[BEST_SOLUTION];
}

void printSolution(const Chromosome& solution, int generation)
{
   std::cout << "Generation: " << generation + 1 << '\n';
   std::cout << "\tGenes: ";
   std::cout << solution << '\n';
   std::cout << "\tFitness: " << solution.get_fitness() << '\n';
}

void printResults(std::vector<Chromosome>& solutions, const Parameters& p)
{
   std::sort(solutions.begin(), solutions.end());

   std::cout << "\n\n\n\n\t\tResults:\n\n";

   std::cout << "Benchmark Function: " << p.target_function << '\n';

   std::cout << "Best Solution Found:\n";
   std::cout << "\t Genes: " << solutions[BEST_SOLUTION];
   std:: cout << "\n\t Fitness: " << solutions[BEST_SOLUTION].get_fitness();
}

/// @brief Parallelized code doesn't work if odd population number.
///
/// This function increases population size by one.
void adjustParallelPopulation(Parameters& p)
{
   p.pop_size += 1;
}
