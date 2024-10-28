#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <unordered_map>

#include "GeneticAlgorithmTSP.h"

namespace ga {

    std::vector<std::vector<double>> distanceMatrix(NUM_CITIES, std::vector<double>(NUM_CITIES));

    // Вывод матрицы в консоль
    void printMatrix(std::vector<std::vector<double>> matrix)
    {
        for (int i = 0; i != matrix.size(); ++i)
        {
            for (int j = 0; j != matrix[0].size(); ++j)
            {

                std::cout << (matrix[i][j]) << " ";
            }
            std::cout << std::endl;
        }
    }

    // Вычисление длины маршрута
    double calculatePathLength(const std::vector<int>& path)
    {
        double length = 0;
        for (int i = 0; i != path.size() - 1; ++i)
        {
            length += distanceMatrix[path[i]][path[i + 1]];
        }
        length += distanceMatrix[path.back()][path[0]];
        return length;
    }

    // Вычисление целевой функции 
    void evaluateFitness(std::vector<Individual>& population)
    {
        for (auto& indiv : population) {
            indiv.fitness = 1.0 / calculatePathLength(indiv.path); // Значение целевой функции обратно пропорционально длине маршрута
        }
    }

    // Поиск лучшей особи в популяции
    Individual bestIndividual(const std::vector<Individual>& population)
    {
        Individual best = population[0];
        for (int i = 1; i != population.size(); ++i)
        {
            if (population[i].fitness > best.fitness)
                best = population[i];
        }
        return best;
    }

    // Инициализация начальной популяции, состоящей из POPULATION_SIZE хромосом
    std::vector<Individual> initializePopulation()
    {
        std::vector<Individual> population;
        std::vector<int> initialPath(NUM_CITIES);
        for (int i = 0; i < NUM_CITIES; ++i)
        {
            initialPath[i] = i;
        }

        // Перестановки случайным образом
        for (int i = 0; i < POPULATION_SIZE; ++i)
        {
            std::random_shuffle(initialPath.begin(), initialPath.end());
            Individual indiv = { initialPath, 1.0 / calculatePathLength(initialPath) };
            population.push_back(indiv);
        }

        return population;
    }

    // Селекция турнирным методом
    std::vector<Individual> selection(const std::vector<Individual>& population)
    {
        std::vector<Individual> selected;

        for (int i = 0; i != POPULATION_SIZE; ++i)
        {
            std::vector<Individual> tournament;

            // Случайным образом выбираем участников турнира
            for (int j = 0; j != TOURNAMENT_SIZE; ++j) {
                int index = rand() % POPULATION_SIZE;
                tournament.push_back(population[index]);
            }

            // Находим лучшего индивидуума в турнире
            Individual best = *std::max_element(tournament.begin(), tournament.end(), [](const Individual& a, const Individual& b) {
                return a.fitness < b.fitness; // Сравниваем значение целевой функции
                });

            selected.push_back(best); // Добавляем лучшего из турнира в список отобранных
        }

        return selected;
    }

    // Скрещивание PMX-методом
    std::vector<Individual> crossover(const Individual& parent1, const Individual& parent2)
    {
        if ((double)rand() / RAND_MAX < CROSSOVER_RATE) // Скрещивание происходит с вероятностью CROSSOVER_RATE
        {
            // Выбираем 2 точки разбиения 
            int point1 = rand() % NUM_CITIES;
            int point2 = rand() % NUM_CITIES;
            if (point1 > point2)
                std::swap(point1, point2);

            std::vector<Individual> children;
            children.push_back(crossoverHelper(parent1, parent2, point1, point2)); // Находим первого потомка
            children.push_back(crossoverHelper(parent2, parent1, point1, point2)); // Находим второго потомка, меняя родителей местами
            return children;
        }
        else
            return { parent1, parent2 };
    }

    // Функция, содержащая основную логику скрещивания
    Individual crossoverHelper(const Individual& parent1, const Individual& parent2, int point1, int point2)
    {
        std::unordered_map<int, int> positions;
        std::vector<int> child(parent2.path); // Инициализируем потомка значениями 2-го родителя

        // Задаем хеш-таблицу, где ключом является элемент, а значение - его позиция во 2-м родителе
        for (int i = 0; i != child.size(); ++i)
        {
            positions[child[i]] = i;
        }

        // Переносим узлы 1-го родителя из центра разбиения в соответствующие позиции в потомке
        for (int i = point1; i != point2; ++i)
        {
            int cur = parent1.path[i];
            int pos = positions[cur];

            positions[cur] = i;
            positions[child[i]] = pos; // Обновляем позиции в хеш-таблице

            std::swap(child[i], child[pos]);
        }

        return { child, -1 };
    }

    // Мутация путем перестановки 2-х случайных узлов 
    void mutate(Individual& indiv) {
        if ((double)rand() / RAND_MAX < MUTATION_RATE) // Мутация с вероятностью MUTATION_RATE
        {
            int i = rand() % NUM_CITIES;
            int j = rand() % NUM_CITIES;
            std::swap(indiv.path[i], indiv.path[j]);
        }
    }

    // Запуск генетического алгоритма
    Individual geneticAlgorithm()
    {

        srand(static_cast<unsigned>(time(0)));
        std::vector<Individual> population = initializePopulation(); // Задаем начальную популяцию 
        auto best = bestIndividual(population);
        std::cout << "Поколение " << 0 << " Лучший путь: " << 1.0 / best.fitness << std::endl;

        for (int generation = 0; generation != GENERATIONS; ++generation)
        {
            std::vector<Individual> selected = selection(population); // Селекция 


            // Создание новой популяции
            std::vector<Individual> newPopulation;
            for (int i = 0; i + 1 < selected.size(); i += 2)
            {
                std::vector<Individual> children = crossover(selected[i], selected[i + 1]); // Попарное скрещивание отобранных особей
                mutate(children[0]);
                mutate(children[1]); // Мутации
                newPopulation.push_back(children[0]);
                newPopulation.push_back(children[1]); // Добавление полученных потомков в новую популяцию
            }
            if (POPULATION_SIZE % 2 == 1) // Если размер популяции нечетный, то добавить еще одного потомка в новую популяцию
            {
                std::vector<Individual> children = crossover(selected[0], selected.back());
                mutate(children[0]);
                newPopulation.push_back(children[0]);
            }

            evaluateFitness(newPopulation); // Оценка приспособленности особей нового поколения

            // Выводми номер поколения и лучший маршрут в этом поколении, переходим к генерации следующего поколению
            population = newPopulation;
            best = bestIndividual(population);
            std::cout << "Поколение " << generation + 1 << " Лучший путь: " << 1.0 / best.fitness << std::endl;
        }
        return best;
    }

} // namespace tsp
