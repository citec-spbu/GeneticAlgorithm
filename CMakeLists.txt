cmake_minimum_required(VERSION 3.10)
project(TSPSolverLib)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_library(tsp_solver
    src/GeneticAlgorithmTSP.cpp
    src/SimulatedAnnealing.cpp
    src/AntColony.cpp
)

add_executable(example example/main.cpp)
target_link_libraries(example tsp_solver)
