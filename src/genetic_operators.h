#pragma once

#include <iostream>
#include <vector>
#include "Chromosome.h"
#include "Parameters.h"

std::vector<Chromosome> initialization(TargetFunction target_fnc, int dimensions, int populationSize);
void evaluatePopulation(std::vector<Chromosome>& population, TargetFunction target_fnc);
const Chromosome& selection(const std::vector<Chromosome>& population, int populationSize, SelectionMethod method, int numCandidates = 3);
std::pair<Chromosome, Chromosome> crossover(const Chromosome& parent1, const Chromosome& parent2, Points nPoints);
void mutation(Chromosome& child1, Chromosome& child2, const Parameters& p);
// void mutation(Chromosome& child, const Parameters& p);
std::vector<Chromosome> createNewGeneration(const std::vector<Chromosome>& prev_gen, const Parameters& p);
std::vector<Chromosome> parallelCreateNewGeneration(const std::vector<Chromosome>& prev_gen, const Parameters& p, int numThreads);
