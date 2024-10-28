#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "GeneticAlgorithmTSP.h"

using namespace ga;

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(0)));

    // Инициализируем матрицу расстояний случайными значениями
    for (int i = 0; i != NUM_CITIES; ++i)
    {
        for (int j = 0; j != NUM_CITIES; ++j)
        {
            if (i == j)
                distanceMatrix[i][j] = INT_MAX;
            else
                distanceMatrix[i][j] = rand() % 100 + 1; // Случайное расстояние от 1 до 100
        }
    }

    Individual bestSolution = geneticAlgorithm();

    std::cout << "Лучший найденный путь: ";
    for (int city : bestSolution.path)
    {
        std::cout << city << " ";
    }
    std::cout << std::endl << "Длина пути: " << calculatePathLength(bestSolution.path) << std::endl;

    return 0;
}
