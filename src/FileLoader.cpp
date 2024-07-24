#include "FileLoader.h"

// -------------------------------------------------------------------------------------------------------------------------------------

std::unordered_map<std::string, TargetFunction> targetFunctionMap 
{
    {"rastrigin", TargetFunction::rastrigin},
    {"ackley", TargetFunction::ackley},
    {"sphere", TargetFunction::sphere},
    {"easom", TargetFunction::easom},
    {"mccormick", TargetFunction::mccormick}
};

std::unordered_map<std::string, SelectionMethod> selectionMethodMap 
{
    {"tournament", SelectionMethod::tournament},
    {"fps", SelectionMethod::fps},
    {"ranking", SelectionMethod::ranking}
};

std::unordered_map<std::string, Points> pointsMap
{
    {"one", Points::one},
    {"two", Points::two},
    {"uniform", Points::uniform}
};

std::string toLower(const std::string_view str) 
{
    std::string lowerStr{ str };

    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
            [](unsigned char c) { return std::tolower(c); });

    return lowerStr;
}

// -------------------------------------------------------------------------------------------------------------------------------------

TargetFunction FileLoader::getTargetFunction(const std::string_view str) 
{
    std::string lowerStr{ toLower(str) };
    return targetFunctionMap[lowerStr];
}

SelectionMethod FileLoader::getSelectionMethod(const std::string_view str) 
{
    std::string lowerStr{ toLower(str) };
    return selectionMethodMap[lowerStr];
}

Points FileLoader::getPoints(const std::string_view str) 
{
    std::string lowerStr{ toLower(str) };
    return pointsMap[lowerStr];
}

Parameters FileLoader::loadFromTXT(const std::string& filePath) 
{
    Parameters params{};
    std::ifstream file(filePath);
    std::string line{};

    if (file.is_open()) 
    {
        while (std::getline(file, line)) 
        {
            std::istringstream ss(line);
            std::string key{};

            if (std::getline(ss, key, '=')) 
            {
                std::string value{};

                if (std::getline(ss, value)) 
                {
                    std::string lowerKey{ toLower(key) };

                    if (lowerKey == "niterations") 
                        params.nIterations = std::stoi(value);
                    else if (lowerKey == "pop_size") 
                        params.pop_size = std::stoi(value);
                    else if (lowerKey == "initial_mutation_rate") 
                        params.initial_mutation_rate = std::stod(value);
                    else if (lowerKey == "final_mutation_rate")
                        params.final_mutation_rate = std::stod(value);
                    else if (lowerKey == "initial_mutation_strength")
                        params.initial_mutation_strength = std::stod(value);
                    else if (lowerKey == "final_mutation_strength") 
                        params.final_mutation_strength = std::stod(value);
                    else if (lowerKey == "elite_fraction") 
                        params.elite_fraction = std::stod(value);
                    else if (lowerKey == "target_function") 
                        params.target_function = getTargetFunction(value);
                    else if (lowerKey == "dimensions")
                        params.dimensions = std::stoi(value);    
                    else if (lowerKey == "method") 
                        params.method = getSelectionMethod(value);
                    else if (lowerKey == "points") 
                        params.points = getPoints(value);
                    else if (lowerKey == "print_precision") 
                        params.print_precision = std::stoi(value);
                    else if (lowerKey == "num_tests") 
                        params.num_tests = std::stoi(value);
                }
            }
        }
        file.close();
        params.mutation_rate = params.initial_mutation_rate;
        params.mutation_strength = params.initial_mutation_strength;
    } 

    else 
    {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
    
    return params;
}
