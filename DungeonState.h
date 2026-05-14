#pragma once
#include "Room.h"
#include <vector>
#include <array>

enum class Visibility { UNKNOWN, NUMBERED, VISIBLE, VISITED };

struct DungeonState {
    std::vector<Room> rooms;
    std::vector<std::array<bool,4>> collectedFromRoom;
    std::vector<Visibility> visibility;
    std::vector<bool> firstCollectUsed;

    int currentRoom = 0;
    int food = 0;
    Resource questResource = Resource::NONE;

    int collectedIron = 0;
    int collectedGold = 0;
    int collectedGems = 0;
    int collectedExp = 0;

    int roomCount() const;
    int resourceValue(Resource r) const;
    void visit(int roomId);
    bool move(int targetRoom);
    Resource collectBest();
    int totalValue() const;
};
