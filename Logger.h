#pragma once
#include "DungeonState.h"
#include <fstream>
#include <string>

class Logger {
public:
    explicit Logger(const std::string& filename);

    void logMove(int roomId);
    void logCollect(Resource r);
    void logState(const DungeonState& ds);
    void logResult(const DungeonState& ds);
    void logError(const std::string& line);

private:
    std::ofstream fout;

    static std::string fmtRes(int current, bool everCollected);
};
