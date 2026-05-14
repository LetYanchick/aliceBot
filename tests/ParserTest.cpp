#include <gtest/gtest.h>
#include "Parser.h"
#include <fstream>

// Write a temp file and parse it
static DungeonState parseString(const std::string& content, const std::string& fname = "test_tmp.txt") {
    std::ofstream f(fname);
    f << content;
    f.close();
    return Parser::parse(fname);
}

static std::string badLine(const std::string& content, const std::string& fname = "test_tmp.txt") {
    std::ofstream f(fname);
    f << content;
    f.close();
    try {
        Parser::parse(fname);
        return "";
    } catch (const ParseError& e) {
        return e.line;
    }
}

// ---- valid input ----

TEST(ParserTest, ParsesExampleCorrectly) {
    auto ds = parseString(
        "5\n"
        "0 1,2 0 0 0 0\n"
        "1 0,3 5 2 1 15\n"
        "2 0,4 3 2 1 10\n"
        "3 1,4 1 0 2 40\n"
        "4 2,5 2 4 0 15\n"
        "5 4 0 5 4 10\n"
        "6 gems\n"
    );

    EXPECT_EQ(ds.food, 6);
    EXPECT_EQ(ds.questResource, Resource::GEMS);
    EXPECT_EQ(ds.rooms[1].iron, 5);
    EXPECT_EQ(ds.rooms[3].gems, 2);
    EXPECT_EQ(ds.rooms[5].gold, 5);
}

TEST(ParserTest, GraphMadeUndirected) {
    // Room 4 lists only {2,5} but room 3 lists {1,4} — after parse, 4 should know about 3
    auto ds = parseString(
        "5\n"
        "0 1,2 0 0 0 0\n"
        "1 0,3 0 0 0 0\n"
        "2 0,4 0 0 0 0\n"
        "3 1,4 0 0 0 0\n"
        "4 2,5 0 0 0 0\n"
        "5 4 0 0 0 0\n"
        "6 gold\n"
    );
    auto& adj4 = ds.rooms[4].adjacent;
    EXPECT_NE(std::find(adj4.begin(), adj4.end(), 3), adj4.end());
}

// ---- invalid input ----

TEST(ParserTest, BadSeparatorInAdjacent) {
    std::string bad = badLine(
        "5\n"
        "0 1,2 0 0 0 0\n"
        "1 0,3 5 2 1 15\n"
        "2 0,4 3 2 1 10\n"
        "3 1,4 1 0 2 40\n"
        "4 2|5 2 4 0 15\n"
        "5 4 0 5 4 10\n"
        "6 gems\n"
    );
    EXPECT_EQ(bad, "4 2|5 2 4 0 15");
}

TEST(ParserTest, MissingResourceFields) {
    std::string bad = badLine(
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0 5 2\n"
        "2 1 0 0 0 0\n"
        "4 gold\n"
    );
    EXPECT_EQ(bad, "1 0 5 2");
}

TEST(ParserTest, UnknownQuestResource) {
    std::string bad = badLine(
        "1\n"
        "0 1 0 0 0 0\n"
        "1 0 0 0 0 0\n"
        "4 diamond\n"
    );
    EXPECT_EQ(bad, "4 diamond");
}

TEST(ParserTest, FoodOutOfRange) {
    std::string bad = badLine(
        "1\n"
        "0 1 0 0 0 0\n"
        "1 0 0 0 0 0\n"
        "1 gold\n"  // M=1 < 2
    );
    EXPECT_EQ(bad, "1 gold");
}

TEST(ParserTest, NOutOfRange) {
    std::string bad = badLine("0\n4 gold\n");
    EXPECT_FALSE(bad.empty());
}
