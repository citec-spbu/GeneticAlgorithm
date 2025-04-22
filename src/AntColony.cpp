#include "AntColony.h"

AntColony::AntColony(const std::vector<std::vector<double>>& distMatrix, int numAnts, 
                                             int maxIterations, double alpha, double beta, double evaporationRate, double q): 
      distanceMatrix(distMatrix), 
      numAnts(numAnts), 
      maxIterations(maxIterations), 
      alpha(alpha), 
      beta(beta), 
      evaporationRate(evaporationRate), 
      q(q), 
      bestPathLength(std::numeric_limits<double>::infinity()) 
{
    pheromones = std::vector<std::vector<double>>(distanceMatrix.size(), std::vector<double>(distanceMatrix.size(), 1.0)); 
}

// Вычисление длины маршрута
double AntColony::calculatePathLength(const std::vector<int>& path) 
{
    double length = 0;
    for (int i = 0; i != path.size() - 1; ++i) 
    {
        length += distanceMatrix[path[i]][path[i + 1]];
    }
    length += distanceMatrix[path.back()][path[0]];
    return length;
}

void AntColony::placePheromones(const std::vector<int>& path, double pathLength) 
{
    // Если путь содержит какое-то ребро, то количество феромона на этом ребре увеличивается 
    double pheromoneDeposit = q / pathLength;
    for (size_t i = 0; i < path.size() - 1; ++i) 
    {
        pheromones[path[i]][path[i + 1]] += pheromoneDeposit;
        pheromones[path[i + 1]][path[i]] += pheromoneDeposit;
    }
    pheromones[path.back()][path[0]] += pheromoneDeposit;
    pheromones[path[0]][path.back()] += pheromoneDeposit;
}

// Обновление феромонов 
void AntColony::updatePheromones(const std::vector<std::vector<int>>& paths, const std::vector<double>& pathLengths) 
{
    for (size_t i = 0; i < pheromones.size(); ++i) 
    {
        for (size_t j = 0; j < pheromones[i].size(); ++j) 
        {
            pheromones[i][j] *= (1.0 - evaporationRate);
        }
    }

    for (size_t i = 0; i < paths.size(); ++i) 
    {
        placePheromones(paths[i], pathLengths[i]);
    }
}

// Построение маршрута муравья
std::vector<int> AntColony::constructSolution() 
{
    std::vector<int> path;
    std::vector<bool> visited(distanceMatrix.size(), false);
    int startCity = rand() % distanceMatrix.size();
    path.push_back(startCity);
    visited[startCity] = true;

    for (size_t step = 1; step != distanceMatrix.size(); ++step) 
    {
        int currentCity = path.back();
        std::vector<double> probabilities(distanceMatrix.size(), 0.0);
        double totalProbability = 0.0;

        for (size_t nextCity = 0; nextCity < distanceMatrix.size(); ++nextCity) 
        {
            if (!visited[nextCity]) 
            {
                double pheromone = pow(pheromones[currentCity][nextCity], alpha);
                double distance = pow(1.0 / distanceMatrix[currentCity][nextCity], beta);
                probabilities[nextCity] = pheromone * distance;
                totalProbability += probabilities[nextCity];
            }
        }

        double randomChoice = ((double)rand() / RAND_MAX) * totalProbability;
        double cumulativeProbability = 0.0;

        for (size_t nextCity = 0; nextCity < distanceMatrix.size(); ++nextCity) 
        {
            if (!visited[nextCity]) 
            {
                cumulativeProbability += probabilities[nextCity];
                if (cumulativeProbability >= randomChoice) 
                {
                    path.push_back(nextCity);
                    visited[nextCity] = true;
                    break;
                }
            }
        }
    }

    return path;  // Возвращаем путь текущего муравья
}


std::vector<int> AntColony::solve() 
{
    for (int iteration = 0; iteration != maxIterations; ++iteration) 
    {
        std::vector<std::vector<int>> paths;
        std::vector<double> pathLengths;

        for (int ant = 0; ant < numAnts; ++ant) 
        {
            std::vector<int> antPath = constructSolution();  // Строим путь муравья
            double pathLength = calculatePathLength(antPath);
            paths.push_back(antPath);
            pathLengths.push_back(pathLength);

            // Обновляем лучший путь
            if (pathLength < bestPathLength) 
            {
                bestPathLength = pathLength;
                bestPath = antPath;
            }
        }

        // Обновляем феромоны для всех муравьев
        updatePheromones(paths, pathLengths);

        // Отладочная информация
        std::cout << "Итерация " << iteration << " Лучший путь: " << bestPathLength << "\n";
    }

    return bestPath;
}


