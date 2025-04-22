

## Подключение в Visual Studio

1. Добавить путь к `include/` в настройки компилятора  
2. Добавить путь к `lib/` в настройки линковщика  
3. Добавить `tsp_solver.lib` в список подключаемых библиотек


## Пример использования

```cpp
#include "tsp_solver.h"

int main() {
    std::vector<std::vector<double>> distances = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    GeneticAlgorithm ga(distances, 100, 1000, 0.2, 0.95, 5);
    auto bestPath = ga.solve();

    for (int city : bestPath)
        std::cout << city << " ";
}
```

