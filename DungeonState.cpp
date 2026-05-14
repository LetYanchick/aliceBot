#include "DungeonState.h"

int DungeonState::roomCount() const {
    return (int)rooms.size();
}

int DungeonState::resourceValue(Resource r) const {
    int base = resourceBaseValue(r);
    return (r == questResource) ? base * 2 : base;
}

void DungeonState::visit(int roomId) {
    visibility[roomId] = Visibility::VISITED;
    for (int nb : rooms[roomId].adjacent) {
        if (visibility[nb] < Visibility::VISIBLE) {
            visibility[nb] = Visibility::VISIBLE;
            for (int nb2 : rooms[nb].adjacent) {
                if (visibility[nb2] == Visibility::UNKNOWN)
                    visibility[nb2] = Visibility::NUMBERED;
            }
        }
    }
}

bool DungeonState::move(int targetRoom) {
    food--;
    currentRoom = targetRoom;
    return food >= 0;
}

Resource DungeonState::collectBest() {
    Room& r = rooms[currentRoom];

    Resource best = Resource::NONE;
    int bestVal = -1;
    for (auto res : { Resource::GEMS, Resource::GOLD, Resource::IRON, Resource::EXP }) {
        if (r.resourceCount(res) > 0) {
            int v = resourceValue(res);
            if (v > bestVal) { 
                bestVal = v; 
                best = res; 
            }
        }
    }
    if (best == Resource::NONE) 
        return Resource::NONE;
    if (firstCollectUsed[currentRoom]) {
        if (food <= 0) 
            return Resource::NONE;
        food--;
    } 
    else {
        firstCollectUsed[currentRoom] = true;
    }

    int count = r.resourceCount(best);
    r.resourceRef(best) = 0;
    collectedFromRoom[currentRoom][static_cast<int>(best)] = true;

    switch (best) {
        case Resource::IRON: 
            collectedIron += count; 
            break;
        case Resource::GOLD: 
            collectedGold += count; 
            break;
        case Resource::GEMS: 
            collectedGems += count; 
            break;
        case Resource::EXP:  
            collectedExp  += count; 
            break;
        default: 
            break;
    }
    return best;
}

int DungeonState::totalValue() const {
    return collectedIron * resourceValue(Resource::IRON) + collectedGold * resourceValue(Resource::GOLD) + collectedGems * resourceValue(Resource::GEMS) + collectedExp * resourceValue(Resource::EXP);
}
