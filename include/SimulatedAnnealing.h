#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <iostream>

class SimulatedAnnealing {
public:
    SimulatedAnnealing(const std::vector<std::vector<double>>& distanceMatrix, double initialTemp = 10000, 
                       double coolingRate = 0.9999, int iterations = 300000);
    std::vector<int> solve();
    double calculatePathLength(const std::vector<int>& path);

private:
    const std::vector<std::vector<double>>& distanceMatrix;
    double initialTemp;
    double coolingRate;
    int iterations;

    std::vector<int> generateInitialSolution();
    std::vector<int> getNeighbor(const std::vector<int>& currentPath);
    double getAcceptanceProbability(double currentDistance, double newDistance, double temperature);
};

#endif
