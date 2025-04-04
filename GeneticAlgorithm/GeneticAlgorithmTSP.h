#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <random>

class GeneticAlgorithm {
public:
    GeneticAlgorithm(const std::vector<std::vector<double>>& distanceMatrix, int populationSize = 500, 
                     int generations = 1000, double mutationRate = 0.2, double crossoverRate = 0.95, int tournamentSize = 7);
    std::vector<int> solve();
    double calculatePathLength(const std::vector<int>& path);

private:
    int numCities;               // Количество городов
    int populationSize;          // Размер популяции
    int generations;             // Количество поколений
    double mutationRate;         // Вероятность мутации
    double crossoverRate;        // Вероятность скрещивания
    int tournamentSize;          // Размер турнира

    std::vector<std::vector<double>> distanceMatrix; // Матрица расстояний

    struct Individual {
        std::vector<int> path;    // Путь обхода городов
        double fitness;           // Приспособленность (целевая функция), обратно пропорциональная длине маршрута
    };

    std::vector<Individual> initializePopulation();
    void evaluateFitness(std::vector<Individual>& population);
    Individual bestIndividual(const std::vector<Individual>& population);
    std::vector<Individual> selection(const std::vector<Individual>& population);
    std::vector<Individual> crossover(const Individual& parent1, const Individual& parent2);
    Individual crossoverHelper(const Individual& parent1, const Individual& parent2, int point1, int point2);
    void mutate(Individual& indiv);
    void printBestSolution(int generation, const Individual& best);

};

#endif