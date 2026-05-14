#pragma once
#include <vector>
#include <string>

enum class Resource { IRON, GOLD, GEMS, EXP, NONE };

std::string resourceName(Resource r);
Resource resourceFromString(const std::string& s);
int resourceBaseValue(Resource r);

struct Room {
    int id = 0;
    std::vector<int> adjacent;
    int iron = 0, gold = 0, gems = 0, exp = 0;

    int& resourceRef(Resource r);
    int resourceCount(Resource r) const;
};
