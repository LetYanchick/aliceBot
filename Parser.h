#pragma once
#include "DungeonState.h"
#include <string>
#include <vector>

struct ParseError {
    std::string line;
    explicit ParseError(const std::string& l) : line(l) {}
};

class Parser {
public:
    static DungeonState parse(const std::string& filename);

private:
    static int parseN(const std::string& line);
    static void parseRoom(const std::string& line, std::vector<Room>& rooms, int N);
    static std::vector<int> parseAdjacent(const std::string& token, const std::string& rawLine, int N);
    static void parseConfig(const std::string& line, DungeonState& ds);
    static std::string trimmed(const std::string& s);
};
