#include "Pathfinder.h"
#include <queue>
#include <algorithm>

std::vector<int> Pathfinder::shortestVisited(const DungeonState& ds, int start, int target) {
    int n = ds.roomCount();
    std::vector<int> prev(n, -1);
    std::vector<bool> seen(n, false);

    std::queue<int> q;
    q.push(start);
    seen[start] = true;

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (cur == target) break;

        std::vector<int> nbrs;
        for (int nb : ds.rooms[cur].adjacent)
            if (ds.visibility[nb] == Visibility::VISITED && !seen[nb])
                nbrs.push_back(nb);
        std::sort(nbrs.begin(), nbrs.end());

        for (int nb : nbrs) {
            seen[nb] = true;
            prev[nb] = cur;
            q.push(nb);
        }
    }

    if (!seen[target]) return {};

    std::vector<int> path;
    for (int cur = target; cur != start; cur = prev[cur])
        path.push_back(cur);
    std::reverse(path.begin(), path.end());
    return path;
}

int Pathfinder::distanceVisited(const DungeonState& ds, int start, int target) {
    return (int)shortestVisited(ds, start, target).size();
}
