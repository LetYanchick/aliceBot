#include "Parser.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

DungeonState Parser::parse(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open())
        throw ParseError("cannot open file: " + filename);

    std::string line;

    if (!std::getline(fin, line)) throw ParseError(line);
    int N = parseN(line);

    DungeonState ds;
    ds.rooms.resize(N + 1);
    ds.visibility.resize(N + 1, Visibility::UNKNOWN);
    ds.firstCollectUsed.resize(N + 1, false);
    ds.collectedFromRoom.resize(N + 1);

    for (int i = 0; i <= N; i++) {
        if (!std::getline(fin, line)) throw ParseError(line);
        parseRoom(line, ds.rooms, N);
    }

    if (!std::getline(fin, line)) throw ParseError(line);
    parseConfig(line, ds);

    // Make graph undirected
    for (int i = 0; i <= N; i++) {
        for (int nb : ds.rooms[i].adjacent) {
            auto& adj = ds.rooms[nb].adjacent;
            if (std::find(adj.begin(), adj.end(), i) == adj.end()) {
                adj.push_back(i);
                std::sort(adj.begin(), adj.end());
            }
        }
    }

    return ds;
}

int Parser::parseN(const std::string& line) {
    std::istringstream ss(trimmed(line));
    int n;
    std::string extra;
    if (!(ss >> n)) 
        throw ParseError(line);
    if (ss >> extra) 
        throw ParseError(line);
    if (n < 1 || n > 255) 
        throw ParseError(line);
    return n;
}

void Parser::parseRoom(const std::string& rawLine, std::vector<Room>& rooms, int N) {
    std::istringstream ss(trimmed(rawLine));

    int id;
    if (!(ss >> id)) 
        throw ParseError(rawLine);
    if (id < 0 || id > N)
        throw ParseError(rawLine);

    std::string adjToken;
    if (!(ss >> adjToken))
        throw ParseError(rawLine);
    std::vector<int> adj = parseAdjacent(adjToken, rawLine, N);

    int iron, gold, gems, exp;
    if (!(ss >> iron >> gold >> gems >> exp)) 
        throw ParseError(rawLine);

    std::string extra;
    if (ss >> extra) 
        throw ParseError(rawLine);

    if (iron < 0 || iron > 255) 
        throw ParseError(rawLine);
    if (gold < 0 || gold > 255) 
        throw ParseError(rawLine);
    if (gems < 0 || gems > 255) 
        throw ParseError(rawLine);
    if (exp  < 0 || exp  > 255) 
        throw ParseError(rawLine);

    rooms[id] = { id, adj, iron, gold, gems, exp };
}

std::vector<int> Parser::parseAdjacent(const std::string& token, const std::string& rawLine, int N) {
    for (char c : token)
        if (!isdigit(c) && c != ',') 
            throw ParseError(rawLine);

    std::vector<int> result;
    std::istringstream ss(token);
    std::string part;
    while (std::getline(ss, part, ',')) {
        if (part.empty()) 
            throw ParseError(rawLine);
        int v = std::stoi(part);
        if (v < 0 || v > N) 
            throw ParseError(rawLine);
        result.push_back(v);
    }
    std::sort(result.begin(), result.end());
    return result;
}

void Parser::parseConfig(const std::string& rawLine, DungeonState& ds) {
    std::istringstream ss(trimmed(rawLine));
    int m;
    std::string res;
    if (!(ss >> m >> res)) 
        throw ParseError(rawLine);
    std::string extra;
    if (ss >> extra) 
        throw ParseError(rawLine);
    if (m < 2 || m > 255) 
        throw ParseError(rawLine);
    Resource r = resourceFromString(res);
    if (r == Resource::NONE) 
        throw ParseError(rawLine);
    ds.food = m;
    ds.questResource = r;
}

std::string Parser::trimmed(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}
