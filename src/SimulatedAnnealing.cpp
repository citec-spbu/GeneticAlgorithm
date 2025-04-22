#include "SimulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing(const std::vector<std::vector<double>>& distanceMatrix, double initialTemp, 
                                       double coolingRate, int iterations): 
      distanceMatrix(distanceMatrix), 
      initialTemp(initialTemp), 
      coolingRate(coolingRate), 
      iterations(iterations) {}

// Генерация начального решения
std::vector<int> SimulatedAnnealing::generateInitialSolution() 
{
    std::vector<int> path(distanceMatrix.size());
    for (size_t i = 0; i < path.size(); ++i)
        path[i] = i;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(path.begin(), path.end(), g);
    return path;
}

// Вычисление длины маршрута
double SimulatedAnnealing::calculatePathLength(const std::vector<int>& path) 
{
    double length = 0;
    for (int i = 0; i != path.size() - 1; ++i) 
    {
        length += distanceMatrix[path[i]][path[i + 1]];
    }
    length += distanceMatrix[path.back()][path[0]];
    return length;
}

// Генерация соседнего решения
std::vector<int> SimulatedAnnealing::getNeighbor(const std::vector<int>& currentPath) 
{
    std::vector<int> newPath = currentPath;
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist(1, newPath.size() - 1);

    int i = dist(g);
    int j = dist(g);
    if (i > j) 
        std::swap(i, j);
    std::reverse(newPath.begin() + i, newPath.begin() + j + 1); // Инверсируем подпоследовательность

    return newPath;
}

// Получение вероятности перехода к новому решению 
double SimulatedAnnealing::getAcceptanceProbability(double currentDistance, double newDistance, double temperature) 
{
    if (newDistance < currentDistance)
        return 1.0;
    return std::exp((currentDistance - newDistance) / temperature);
}

// Запуск алгоритма
std::vector<int> SimulatedAnnealing::solve() 
{
    std::vector<int> currentSolution = generateInitialSolution(); // Генерация начального решения
    double currentDistance = calculatePathLength(currentSolution);
    std::vector<int> bestSolution = currentSolution;
    double bestDistance = currentDistance;

    double temperature = initialTemp;
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int iter = 0; iter != iterations; ++iter) 
    {
        std::vector<int> newSolution = getNeighbor(currentSolution); // Генерация соседнего решения
        double newDistance = calculatePathLength(newSolution);

        // С определенной вероятностью переход к новому решению
        if (getAcceptanceProbability(currentDistance, newDistance, temperature) > dist(g)) 
        {
            currentSolution = newSolution;
            currentDistance = newDistance;

            if (newDistance < bestDistance) 
            {
                bestSolution = newSolution;
                bestDistance = newDistance;
            }
        }
        
        if (iter % 1000 == 0)
            std::cout << "Итерация " << iter << " Лучший путь: " << bestDistance << std::endl;

        // Уменьшение температуры 
        temperature *= coolingRate;
    }

    return bestSolution;
}
