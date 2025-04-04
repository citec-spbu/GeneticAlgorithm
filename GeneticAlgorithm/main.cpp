#include <iostream>
#include <random>
#include "GeneticAlgorithmTSP.h"
#include "SimulatedAnnealing.h"
#include "AntColony.h"


// Генерация случайной симметричной матрицы
std::vector<std::vector<double>> generateRandomDistanceMatrix(int size) {
    std::vector<std::vector<double>> matrix(size, std::vector<double>(size, 0.0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(10.0, 1000.0);

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            double distance = dist(gen);
            matrix[i][j] = matrix[j][i] = distance;
        }
    }

    return matrix;
}


int main() 
{
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(0)));

    std::vector<std::vector<double>> distanceMatrix = generateRandomDistanceMatrix(200);

    std::cout << "Генетический алгоритм:\n";
    GeneticAlgorithm ga(distanceMatrix);
    std::vector<int> bestSolution = ga.solve();
    std::cout << "Лучший найденный путь: " << ga.calculatePathLength(bestSolution) << std::endl;

    std::cout << "\nАлгоритм имитации отжига:\n";
    SimulatedAnnealing sa(distanceMatrix);
    std::vector<int> bestPath = sa.solve();
    std::cout << "Лучший найденный путь: " << sa.calculatePathLength(bestPath) << std::endl;

    std::cout << "\nМуравьиный алгоритм:\n";
    AntColony aco(distanceMatrix);
    std::vector<int> bestPathACO = aco.solve();


    std::cout << "Лучший найденный путь: " << aco.calculatePathLength(bestPathACO) << std::endl;

    return 0;
}
