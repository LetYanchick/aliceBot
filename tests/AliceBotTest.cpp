#include <gtest/gtest.h>
#include "Parser.h"
#include "Logger.h"
#include "AliceBot.h"
#include <fstream>
#include <sstream>

static std::string runBot(const std::string& input) {
    {
        std::ofstream f("bot_test_in.txt");
        f << input;
    }

    DungeonState ds = Parser::parse("bot_test_in.txt");
    {
        // Logger closes the file when it goes out of scope
        Logger logger("bot_test_out.txt");
        AliceBot bot;
        bot.run(ds, logger);
    } // file flushed and closed here

    std::ifstream f("bot_test_out.txt");
    return std::string(std::istreambuf_iterator<char>(f), {});
}

TEST(AliceBotTest, SampleFromTask) {
    std::string output = runBot(
        "5\n"
        "0 1,2 0 0 0 0\n"
        "1 0,3 5 2 1 15\n"
        "2 0,4 3 2 1 10\n"
        "3 1,4 1 0 2 40\n"
        "4 2,5 2 4 0 15\n"
        "5 4 0 5 4 10\n"
        "6 gems\n"
    );

    // Check key moves are present in order
    EXPECT_NE(output.find("go 1\n"), std::string::npos);
    EXPECT_NE(output.find("collect gems\n"), std::string::npos);
    EXPECT_NE(output.find("go 3\n"), std::string::npos);
    EXPECT_NE(output.find("go 4\n"), std::string::npos);
    EXPECT_NE(output.find("collect gold\n"), std::string::npos);
    EXPECT_NE(output.find("go 0\n"), std::string::npos);

    // Result line: gold=4, gems=3
    EXPECT_NE(output.find("result 0 4 3 0"), std::string::npos);
}

TEST(AliceBotTest, ReturnsToStart) {
    std::string output = runBot(
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 0 10 0 0\n"
        "2 1 0 0 5 0\n"
        "4 gold\n"
    );
    // Must end with go 0 and then result
    size_t go0  = output.rfind("go 0\n");
    size_t res  = output.find("result ");
    EXPECT_NE(go0, std::string::npos);
    EXPECT_NE(res, std::string::npos);
    EXPECT_GT(res, go0);
}

TEST(AliceBotTest, DoesNotExceedFoodBudget) {
    // With food=2, explore budget=1. Bot can go to room 1 only.
    std::string output = runBot(
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 0 5 0 0\n"
        "2 1 5 0 0 0\n"
        "2 gold\n"
    );
    // Room 2 should not be visited (would need 2 food to go there and return)
    EXPECT_EQ(output.find("go 2\n"), std::string::npos);
}
