#include "Logger.h"
#include <stdexcept>

Logger::Logger(const std::string& filename) {
    fout.open(filename);
    if (!fout.is_open())
        throw std::runtime_error("Cannot open output file: " + filename);
}

void Logger::logMove(int roomId) {
    fout << "go " << roomId << "\n";
}

void Logger::logCollect(Resource r) {
    fout << "collect " << resourceName(r) << "\n";
}

void Logger::logState(const DungeonState& ds) {
    int id = ds.currentRoom;
    const Room& r = ds.rooms[id];
    fout << "state " << id << " "
         << fmtRes(r.iron, ds.collectedFromRoom[id][0]) << " "
         << fmtRes(r.gold, ds.collectedFromRoom[id][1]) << " "
         << fmtRes(r.gems, ds.collectedFromRoom[id][2]) << " "
         << fmtRes(r.exp,  ds.collectedFromRoom[id][3]) << "\n";
}

void Logger::logResult(const DungeonState& ds) {
    fout << "result "
         << ds.collectedIron << " "
         << ds.collectedGold << " "
         << ds.collectedGems << " "
         << ds.collectedExp  << " "
         << ds.totalValue()  << "\n";
}

void Logger::logError(const std::string& line) {
    fout << line << "\n";
}

std::string Logger::fmtRes(int current, bool everCollected) {
    if (everCollected) 
        return "_";
    return std::to_string(current);
}
