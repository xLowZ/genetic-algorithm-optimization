cmake_minimum_required(VERSION 3.25)

# Definir o nome do projeto e a versão
project(gao VERSION 1.0) 

# Especificar o padrão C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Encontrar OpenMP
find_package(OpenMP REQUIRED)

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
    set(OSBitness 64)
endif() 

# Organização
set(FullOutputDir "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}${OSBitness}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}") 

# Adicionar executável
add_executable(${PROJECT_NAME} src/main.cpp src/Chromosome.cpp src/FileLoader.cpp src/genetic_operators.cpp)

# Incluir diretórios de header
include_directories(src/include)

# Configurar diretórios de inclusão para o alvo
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Ligar OpenMP
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

# Definir flags de compilação específicas para MSVC e GCC
# if (MSVC)
#     target_compile_options(${PROJECT_NAME} PRIVATE /W4 /O2 /openmp)
# else()
#     target_compile_options(${PROJECT_NAME} PRIVATE -Wall -O3 -fopenmp)
# endif()

# # Set the build type to Release by default if not specified
# if(NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
# endif()

# # Configurar as flags de compilação para diferentes tipos de build
# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# # Adicionar as flags de compilação ao alvo
# target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:${CMAKE_CXX_FLAGS_DEBUG}>)
# target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>)
