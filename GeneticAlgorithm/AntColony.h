#ifndef ANT_COLONY_OPTIMIZATION_H
#define ANT_COLONY_OPTIMIZATION_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <limits>

class AntColony {
public:
    AntColony(const std::vector<std::vector<double>>& distMatrix, int numAnts = 100, int maxIterations = 50, 
                          double alpha = 1.5, double beta = 1.5, double evaporationRate = 0.5, double q = 500);
    std::vector<int> solve();
    double calculatePathLength(const std::vector<int>& path);

private:
    void updatePheromones(const std::vector<std::vector<int>>& allPaths, const std::vector<double>& allPathLengths);
    std::vector<int> constructSolution();
    void placePheromones(const std::vector<int>& path, double pathLength);
    std::vector<std::vector<double>> distanceMatrix;
    std::vector<std::vector<double>> pheromones;
    std::vector<int> bestPath;
    double bestPathLength;
    int numAnts;
    int maxIterations;
    double alpha;
    double beta;
    double evaporationRate;
    double q;
};

#endif
