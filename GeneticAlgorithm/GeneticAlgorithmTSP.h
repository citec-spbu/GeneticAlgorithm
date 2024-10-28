#ifndef GENETICALGORITHMTSP_H
#define GENETICALGORITHMTSP_H

#include <vector>

namespace ga
{

    const int NUM_CITIES = 50;             // Количество городов
    const int POPULATION_SIZE = 800;       // Размер популяции
    const int GENERATIONS = 70;            // Количество поколений
    const double MUTATION_RATE = 0.35;     // Вероятность мутации
    const double CROSSOVER_RATE = 0.95;    // Вероятность скрещивания
    const int TOURNAMENT_SIZE = 8;         // Размер турнира

    struct Individual
    {
        std::vector<int> path;             // Путь обхода городов
        double fitness;                    // Приспособленность (целевая функция), обратно пропорциональная длине маршрута
    };

    extern std::vector<std::vector<double>> distanceMatrix;

    void printMatrix(std::vector<std::vector<double>> matrix);
    Individual bestIndividual(const std::vector<Individual>& population);
    double calculatePathLength(const std::vector<int>& path);
    std::vector<Individual> initializePopulation();
    void evaluateFitness(std::vector<Individual>& population);
    std::vector<Individual> selection(const std::vector<Individual>& population);
    std::vector<Individual> crossover(const Individual& parent1, const Individual& parent2);
    Individual crossoverHelper(const Individual& parent1, const Individual& parent2, int point1, int point2);
    void mutate(Individual& indiv);
    Individual geneticAlgorithm();

} // namespace ga

#endif 
