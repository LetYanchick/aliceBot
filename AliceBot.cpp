#include "AliceBot.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>

void AliceBot::run(DungeonState& ds, Logger& logger) {
    ds.visit(0);

    int foodForExplore = ds.food / 2;
    int foodSpentExploring = 0;

    // Фаза исследования
    while (true) {
        int nextRoom = chooseNextRoom(ds);
        if (nextRoom == -1) 
            break;

        int moveCost = costToReach(ds, nextRoom);
        if (moveCost == 0) 
            break;
        if (foodSpentExploring + moveCost > foodForExplore) 
            break;

        if (travelTo(ds, logger, nextRoom)) 
            return;
        foodSpentExploring += moveCost;

        collectBestInRoom(ds, logger);
    }

    // --- Phase 2: Return ---
    returnHome(ds, logger);
    logger.logResult(ds);
}

int AliceBot::chooseNextRoom(const DungeonState& ds) {
    // Prefer unvisited adjacent to current room, smallest id first
    std::vector<int> adjUnvisited;
    for (int nb : ds.rooms[ds.currentRoom].adjacent)
        if (ds.visibility[nb] != Visibility::VISITED)
            adjUnvisited.push_back(nb);

    if (!adjUnvisited.empty()) {
        std::sort(adjUnvisited.begin(), adjUnvisited.end());
        return adjUnvisited[0];
    }

    // Otherwise BFS over visited to find nearest unvisited, smallest id on ties
    int n = ds.roomCount();
    std::vector<int> dist(n, -1);
    std::queue<int>  q;
    q.push(ds.currentRoom);
    dist[ds.currentRoom] = 0;

    int bestId   = -1;
    int bestDist = std::numeric_limits<int>::max();

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        for (int nb : ds.rooms[cur].adjacent) {
            if (dist[nb] != -1) continue;
            if (ds.visibility[nb] == Visibility::VISITED) {
                dist[nb] = dist[cur] + 1;
                q.push(nb);
            } else {
                dist[nb] = dist[cur] + 1;
                if (dist[nb] < bestDist || (dist[nb] == bestDist && nb < bestId)) {
                    bestDist = dist[nb];
                    bestId   = nb;
                }
            }
        }
    }
    return bestId;
}

int AliceBot::costToReach(const DungeonState& ds, int targetRoom) {
    if (ds.visibility[targetRoom] == Visibility::VISITED)
        return (int)Pathfinder::shortestVisited(ds, ds.currentRoom, targetRoom).size();

    // BFS over visited; find a visited room that has an edge into targetRoom
    int n = ds.roomCount();
    std::vector<int> dist(n, -1);
    std::queue<int>  q;
    q.push(ds.currentRoom);
    dist[ds.currentRoom] = 0;

    int best = std::numeric_limits<int>::max();
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        for (int nb : ds.rooms[cur].adjacent) {
            if (nb == targetRoom) {
                best = std::min(best, dist[cur] + 1);
                continue;
            }
            if (ds.visibility[nb] == Visibility::VISITED && dist[nb] == -1) {
                dist[nb] = dist[cur] + 1;
                q.push(nb);
            }
        }
    }
    return (best == std::numeric_limits<int>::max()) ? 0 : best;
}

bool AliceBot::travelTo(DungeonState& ds, Logger& logger, int target) {
    while (ds.currentRoom != target) {
        int next = nextStepTo(ds, target);
        if (next == -1) return false;

        logger.logMove(next);
        ds.move(next);
        ds.visit(next);
        logger.logState(ds);

        if (ds.food < 0 && ds.currentRoom != 0) return true;
    }
    return false;
}

int AliceBot::nextStepTo(const DungeonState& ds, int target) {
    if (ds.visibility[target] == Visibility::VISITED) {
        auto path = Pathfinder::shortestVisited(ds, ds.currentRoom, target);
        return path.empty() ? -1 : path[0];
    }

    // BFS over visited to find room with edge into target
    int n = ds.roomCount();
    std::vector<int> dist(n, -1);
    std::vector<int> prev(n, -1);
    std::queue<int>  q;
    q.push(ds.currentRoom);
    dist[ds.currentRoom] = 0;

    int gateway = -1;
    while (!q.empty() && gateway == -1) {
        int cur = q.front(); q.pop();
        for (int nb : ds.rooms[cur].adjacent) {
            if (nb == target) { gateway = cur; break; }
            if (ds.visibility[nb] == Visibility::VISITED && dist[nb] == -1) {
                dist[nb] = dist[cur] + 1;
                prev[nb] = cur;
                q.push(nb);
            }
        }
    }
    if (gateway == -1)              return -1;
    if (gateway == ds.currentRoom)  return target;

    std::vector<int> path;
    for (int cur = gateway; cur != ds.currentRoom; cur = prev[cur])
        path.push_back(cur);
    std::reverse(path.begin(), path.end());
    return path[0];
}

void AliceBot::collectBestInRoom(DungeonState& ds, Logger& logger) {
    Resource r = ds.collectBest();
    if (r != Resource::NONE) {
        logger.logCollect(r);
        logger.logState(ds);
    }
}

void AliceBot::returnHome(DungeonState& ds, Logger& logger) {
    auto path = Pathfinder::shortestVisited(ds, ds.currentRoom, 0);

    for (int i = 0; i < (int)path.size(); i++) {
        int  next   = path[i];
        bool isLast = (next == 0);
        int  stepsToHome = (int)(path.size() - i);

        collectAllAffordable(ds, logger, stepsToHome);

        logger.logMove(next);
        ds.move(next);
        if (!isLast) logger.logState(ds);
    }
}

void AliceBot::collectAllAffordable(DungeonState& ds, Logger& logger, int stepsToHome) {
    while (true) {
        bool firstFree = !ds.firstCollectUsed[ds.currentRoom];
        if (!firstFree && ds.food - 1 < stepsToHome) break;

        Resource r = ds.collectBest();
        if (r == Resource::NONE) break;

        logger.logCollect(r);
        logger.logState(ds);
    }
}
