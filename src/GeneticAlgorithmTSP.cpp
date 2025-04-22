#include "GeneticAlgorithmTSP.h"

GeneticAlgorithm::GeneticAlgorithm(const std::vector<std::vector<double>>& distanceMatrix,
    int populationSize, int generations,
    double mutationRate, double crossoverRate, int tournamentSize): 
    distanceMatrix(distanceMatrix),
    numCities(distanceMatrix.size()),
    populationSize(populationSize),
    generations(generations),
    mutationRate(mutationRate),
    crossoverRate(crossoverRate),
    tournamentSize(tournamentSize) {}

// Инициализация начальной популяции, состоящей из populationSize хромосом
std::vector<GeneticAlgorithm::Individual> GeneticAlgorithm::initializePopulation() 
{
    std::vector<Individual> population;
    std::vector<int> initialPath(numCities);

    for (int i = 0; i < numCities; ++i) 
    {
        initialPath[i] = i;
    }

    // Перестановки случайным образом
    for (int i = 0; i < populationSize; ++i) 
    {
        std::random_shuffle(initialPath.begin(), initialPath.end());
        Individual indiv = { initialPath, 1.0 / calculatePathLength(initialPath) };
        population.push_back(indiv);
    }

    return population;
}

// Вычисление длины маршрута
double GeneticAlgorithm::calculatePathLength(const std::vector<int>& path) 
{
    double length = 0;
    for (int i = 0; i != path.size() - 1; ++i) 
    {
        length += distanceMatrix[path[i]][path[i + 1]];
    }
    length += distanceMatrix[path.back()][path[0]]; 
    return length;
}

// Оценка приспособленности для каждого индивидуума в поколении
void GeneticAlgorithm::evaluateFitness(std::vector<Individual>& population) 
{
    for (auto& indiv : population)
        indiv.fitness = 1.0 / calculatePathLength(indiv.path); // Значение целевой функции обратно пропорционально длине маршрута
}

// Поиск лучшей особи в популяции
GeneticAlgorithm::Individual GeneticAlgorithm::bestIndividual(const std::vector<Individual>& population) 
{
    Individual best = population[0];
    for (int i = 1; i != population.size(); ++i) 
    {
        if (population[i].fitness > best.fitness)
            best = population[i];
    }
    return best;
}

// Турнирная селекция
std::vector<GeneticAlgorithm::Individual> GeneticAlgorithm::selection(const std::vector<Individual>& population) 
{
    std::vector<Individual> selected;
    for (int i = 0; i != populationSize; ++i) 
    {
        std::vector<Individual> tournament;

        // Случайным образом выбираем участников турнира
        for (int j = 0; j != tournamentSize; ++j) {
            int index = rand() % populationSize;
            tournament.push_back(population[index]);
        }

        // Находим лучшего индивидуума в турнире
        Individual best = *std::max_element(tournament.begin(), tournament.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
            });

        selected.push_back(best); // Добавляем лучшего из турнира в список отобранных
    }
    return selected;
}

// Скрещивание OX-методом
std::vector<GeneticAlgorithm::Individual> GeneticAlgorithm::crossover(const Individual& parent1, const Individual& parent2) 
{
    std::vector<Individual> children;
    if ((double)rand() / RAND_MAX < crossoverRate) { // Скрещивание происходит с вероятностью crossoverRate
        // Выбираем 2 точки разбиения
        int point1 = rand() % numCities;
        int point2 = rand() % numCities;
        if (point1 > point2) std::swap(point1, point2);

        // Создаем потомков
        children.push_back(crossoverHelper(parent1, parent2, point1, point2));
        // Меняем родителей местами 
        children.push_back(crossoverHelper(parent2, parent1, point1, point2));
    }
    else {
        children.push_back(parent1);
        children.push_back(parent2);
    }
    return children;
}

// Функция, содержащая основную логику скрещивания
GeneticAlgorithm::Individual GeneticAlgorithm::crossoverHelper(const Individual& parent1, const Individual& parent2, int point1, int point2)
{
    std::vector<int> child(numCities, -1);

    // Копируем сегмент из parent1
    std::unordered_set<int> segment;
    for (int i = point1; i <= point2; ++i) 
    {
        child[i] = parent1.path[i];
        segment.insert(parent1.path[i]);
    }

    // Заполняем остальное из parent2, пропуская дубликаты
    int pos = (point2 + 1) % numCities;
    for (int i = 0; i < numCities; ++i) 
    {
        int city = parent2.path[(point2 + 1 + i) % numCities];
        if (segment.count(city) == 0) 
        {
            child[pos] = city;
            pos = (pos + 1) % numCities;
        }
    }

    return { child, -1 };
}

// Мутация путем инверсии подпоследовательности
void GeneticAlgorithm::mutate(Individual& indiv) 
{
    if ((double)rand() / RAND_MAX < mutationRate) // Мутация происходит с вероятностью mutationRate
    {
        int i = rand() % numCities;
        int j = rand() % numCities;
        if (i > j) std::swap(i, j);
        std::reverse(indiv.path.begin() + i, indiv.path.begin() + j + 1);
    }
}

// Вывод отладочной информации
void GeneticAlgorithm::printBestSolution(int generation, const Individual& best) 
{
    std::cout << "Поколение " << generation << " Лучший путь: " << 1.0 / best.fitness << std::endl;
}

// Запуск генетического алгоритма
std::vector<int> GeneticAlgorithm::solve() 
{
    std::vector<Individual> population = initializePopulation(); // Задаем начальную популяцию 
    Individual best = bestIndividual(population);
    printBestSolution(0, best);

    int noImprovementGenerations = 0; // Счетчик поколений без улучшения
    for (int generation = 1; generation <= generations; ++generation) 
    {
        std::vector<Individual> selected = selection(population); // Селекция

        // Создание новой популяции
        std::vector<Individual> newPopulation;
        for (int i = 0; i < selected.size() - 1; i += 2) 
        {
            std::vector<Individual> children = crossover(selected[i], selected[i + 1]); // Попарное скрещивание отобранных особей
            mutate(children[0]);
            mutate(children[1]); // Мутации
            newPopulation.push_back(children[0]);
            newPopulation.push_back(children[1]); // Добавление полученных потомков в новую популяцию
        }

        if (populationSize % 2 == 1) // Если размер популяции нечетный, то добавить еще одного потомка в новую популяцию
        {
            std::vector<Individual> children = crossover(selected[0], selected.back());
            mutate(children[0]);
            newPopulation.push_back(children[0]);
        }

        evaluateFitness(newPopulation); // Оценка приспособленности особей нового поколения

        population = newPopulation; // Обновляем популяцию
        Individual newBest = bestIndividual(population); // Находим лучшую особь в новом поколении

        if (newBest.fitness > best.fitness) 
        {
            best = newBest;
            noImprovementGenerations = 0;
        }
        else
        {
            ++noImprovementGenerations;
        }

        printBestSolution(generation, best); // Выводим решение для текущего поколения

        if (noImprovementGenerations >= 100) // Если 100 поколений не было улучшений, то алгоритм завершает работу
            break;
    }
    return best.path;
}
