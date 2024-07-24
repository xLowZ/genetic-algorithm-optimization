#pragma once

#include "constants.h"

struct Parameters 
{
   int             nIterations;
   int             pop_size;
   double          initial_mutation_rate;
   double          final_mutation_rate;
   double          initial_mutation_strength;
   double          final_mutation_strength;
   double          elite_fraction;
   TargetFunction  target_function;
   int             dimensions;
   SelectionMethod method;
   Points          points;
   double          mutation_rate;
   double          mutation_strength;
   int             print_precision;
   int             num_tests;
};
