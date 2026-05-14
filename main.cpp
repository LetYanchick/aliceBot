#include "Parser.h"
#include "Logger.h"
#include "AliceBot.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Parameter input_file is needed\n";
        return 1;
    }

    const std::string inputFile = argv[1];
    const std::string outputFile = "result.txt";

    DungeonState ds;
    try {
        ds = Parser::parse(inputFile);
    }
    catch (const ParseError& e) {
        // В result.txt пишется строчка с некоррекным вводом
        Logger errLog(outputFile);
        errLog.logError(e.line);
        return 0;
    }

    Logger logger(outputFile);

    // можно встроить и испытать любой другой алгоритм для бота, работающего в описанных условиях
    std::unique_ptr<Bot> bot = std::make_unique<AliceBot>();
    bot->run(ds, logger);

    return 0;
}
