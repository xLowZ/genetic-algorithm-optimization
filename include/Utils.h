#pragma once

#include <iostream>
#include <iomanip>
#include <omp.h>
// #include <clocale> 

// -------------------------------------------------------------------------------------------------------------------------------------

namespace Settings {

   inline void setup_precision(int precision) 
   {
      std::cout << std::setprecision(precision) << std::fixed;
   }

   namespace MultiThread {
      const int maxThreads{ omp_get_max_threads() };
      constexpr int BASE_POPULATION_PARALLEL_THRESHOLD{ 500 };
      const int POPULATION_PARALLEL_THRESHOLD{ BASE_POPULATION_PARALLEL_THRESHOLD * maxThreads / 4 };
   }

}

// -------------------------------------------------------------------------------------------------------------------------------------

inline void printElapsedTime(double elapsedSeconds) 
{
   int hours { static_cast<int>(elapsedSeconds / 3600) };
   elapsedSeconds -= hours * 3600;
   int minutes { static_cast<int>(elapsedSeconds / 60) };
   elapsedSeconds -= minutes * 60;
   double seconds { elapsedSeconds };

   std::cout << "\nElapsed time: ";
   std::cout << std::setfill('0') << std::setw(2) << hours << " h: "
             << std::setfill('0') << std::setw(2) << minutes << " min: "
             << std::setfill('0') << std::setw(2) << std::fixed << std::setprecision(2) << seconds << 's'
             << std::endl;
}

// -------------------------------------------------------------------------------------------------------------------------------------

inline void print_help() 
{
   // std::setlocale(LC_ALL, "pt_BR.UTF-8");
   std::cout << "\nUsage:\n\t ./gao.exe [config_file_path]>\n\n";
   std::cout << "Description:\n\n";
   std::cout << "  This program uses parameters from a .txt file to run a genetic algorithm.\n";
   std::cout << "  Edit the txt with the following structure:\n\n";
   std::cout << "  Example content of 'parameters.txt':\n\n";
   std::cout << "  nIterations=1000                --> number of generations\n";
   std::cout << "  pop_size=50                     --> number of individuals\n";
   std::cout << "  initial_mutation_rate=0.1\n";
   std::cout << "  final_mutation_rate=0.05\n";
   std::cout << "  initial_mutation_strength=1.0\n";
   std::cout << "  final_mutation_strength=0.2\n";
   std::cout << "  elite_fraction=0.02             --> percentage of individuals to keep in the next generation\n";
   std::cout << "  target_function=mccormick       --> function to optimize\n";
   std::cout << "  dimensions=2\n";
   std::cout << "  selection_method=tournament     --> available:  tournament  |  fps (fitness proportionate selection) |  ranking\n";
   std::cout << "  points=2                        --> crossover methods | available:  one  |  two  |  uniform\n";
   std::cout << "  print_precision=4               --> number of digits to be displayed on terminal\n";
   std::cout << "  num_tests=10                    --> number of times the algorithm will run\n\n";
   std::cout << "  Note: Each parameter should be on a separate line, in the format 'parameter_name=value'.\n";
   std::cout << "  Modify the values as needed for your specific configuration.\n";
}

// -------------------------------------------------------------------------------------------------------------------------------------

inline void printNoArgs()
{
   std::cout << "\n\tInvalid Input.\n";
   std::cout << "\n\tUsage: .\\gao.exe [config_file_path]\n";
   std:: cout << "\n\tRun '.\\gao.exe --help' for more information.\n";
}
