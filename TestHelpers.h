#pragma once
#include "../DungeonState.h"

// Build a minimal DungeonState for tests without going through the Parser.
inline DungeonState makeState(int n, Resource quest, int food) {
    DungeonState ds;
    ds.rooms.resize(n + 1);
    ds.visibility.resize(n + 1, Visibility::UNKNOWN);
    ds.firstCollectUsed.resize(n + 1, false);
    ds.collectedFromRoom.resize(n + 1);
    ds.food          = food;
    ds.questResource = quest;
    for (int i = 0; i <= n; i++) ds.rooms[i].id = i;
    return ds;
}

// Link two rooms bidirectionally.
inline void link(DungeonState& ds, int a, int b) {
    ds.rooms[a].adjacent.push_back(b);
    ds.rooms[b].adjacent.push_back(a);
}
