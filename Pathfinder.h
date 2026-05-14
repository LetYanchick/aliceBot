#pragma once
#include "DungeonState.h"
#include <vector>

class Pathfinder {
public:
    // кратчайший путь через посещенные комнаты от старта до цели
    // Выбирая меньший id комнаты
    // Возврат id комнат которые надо посетить, без стартартовой. Пустой если таргет не достижима
    static std::vector<int> shortestVisited(const DungeonState& ds, int start, int target);

    // Сколько прошли
    static int distanceVisited(const DungeonState& ds, int start, int target);
};
