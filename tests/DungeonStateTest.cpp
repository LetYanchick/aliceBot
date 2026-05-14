#include <gtest/gtest.h>
#include "TestHelpers.h"


// Тесты ресурсов
TEST(DungeonStateTest, ResourceValueDoubledForQuest) {
    auto ds = makeState(1, Resource::GEMS, 10);
    EXPECT_EQ(ds.resourceValue(Resource::GEMS), 46);
    EXPECT_EQ(ds.resourceValue(Resource::GOLD), 11); 
}

TEST(DungeonStateTest, ResourceValueNormalWhenNotQuest) {
    auto ds = makeState(1, Resource::IRON, 10);
    EXPECT_EQ(ds.resourceValue(Resource::IRON), 14); 
    EXPECT_EQ(ds.resourceValue(Resource::GEMS), 23);
}

//Тесты состояний видимости комнат
TEST(DungeonStateTest, VisitUpdatesVisibility) {
    //  0 - 1 - 2 - 3
    auto ds = makeState(3, Resource::GOLD, 10);
    link(ds, 0, 1); 
    link(ds, 1, 2); 
    link(ds, 2, 3);

    ds.visit(0);
    EXPECT_EQ(ds.visibility[0], Visibility::VISITED);
    EXPECT_EQ(ds.visibility[1], Visibility::VISIBLE);   
    EXPECT_EQ(ds.visibility[2], Visibility::NUMBERED);
    EXPECT_EQ(ds.visibility[3], Visibility::UNKNOWN); 
}

TEST(DungeonStateTest, VisitChainExtendsVisibility) {
    auto ds = makeState(3, Resource::GOLD, 10);
    link(ds, 0, 1); 
    link(ds, 1, 2); 
    link(ds, 2, 3);
    ds.visit(0);
    ds.visit(1);
    EXPECT_EQ(ds.visibility[2], Visibility::VISIBLE);
    EXPECT_EQ(ds.visibility[3], Visibility::NUMBERED);
}

// Движения
TEST(DungeonStateTest, MoveDecreasesFood) {
    auto ds = makeState(1, Resource::GOLD, 5);
    link(ds, 0, 1);
    ds.visit(0);

    bool alive = ds.move(1);
    EXPECT_TRUE(alive);
    EXPECT_EQ(ds.food, 4);
    EXPECT_EQ(ds.currentRoom, 1);
}

TEST(DungeonStateTest, MoveReturnsFalseWhenFoodZero) {
    auto ds = makeState(1, Resource::GOLD, 1);
    link(ds, 0, 1);
    ds.visit(0);

    bool alive = ds.move(1); //еды 0, это можно
    EXPECT_TRUE(alive);

    alive = ds.move(0); // еды -1, это уже неправильно
    EXPECT_FALSE(alive);
}


TEST(DungeonStateTest, FirstCollectIsFree) {
    auto ds = makeState(1, Resource::GOLD, 3);
    ds.rooms[0].gems = 2;
    ds.visit(0);
    Resource r = ds.collectBest();
    EXPECT_EQ(r, Resource::GEMS);
    EXPECT_EQ(ds.food, 3); // еды не потрачено
    EXPECT_EQ(ds.collectedGems, 2); // собрали все gems
    EXPECT_EQ(ds.rooms[0].gems, 0);
}

TEST(DungeonStateTest, SecondCollectCostsFood) {
    auto ds = makeState(1, Resource::GOLD, 3);
    ds.rooms[0].gems = 1;
    ds.rooms[0].iron = 5;
    ds.visit(0);

    ds.collectBest(); // gems бесплатные
    EXPECT_EQ(ds.food, 3);

    Resource r = ds.collectBest(); // iron за 1
    EXPECT_EQ(r, Resource::IRON);
    EXPECT_EQ(ds.food, 2);
    EXPECT_EQ(ds.collectedIron, 5);
}

TEST(DungeonStateTest, CollectNoneWhenRoomEmpty) {
    auto ds = makeState(1, Resource::GOLD, 3);
    ds.visit(0);

    Resource r = ds.collectBest();
    EXPECT_EQ(r, Resource::NONE);
    EXPECT_EQ(ds.food, 3);
}

TEST(DungeonStateTest, CollectNoneWhenNoFood) {
    auto ds = makeState(1, Resource::GOLD, 0);
    ds.rooms[0].gems = 1;
    ds.rooms[0].iron = 1;
    ds.visit(0);

    ds.collectBest(); // gems бесплатные
    Resource r = ds.collectBest(); // на iron не хватает
    EXPECT_EQ(r, Resource::NONE);
}

TEST(DungeonStateTest, CollectPrefersHighestValue) {
    auto ds = makeState(1, Resource::GOLD, 5);
    ds.rooms[0].iron = 1;
    ds.rooms[0].gold = 1;
    ds.rooms[0].gems = 1;
    ds.visit(0);

    // gems=23, gold=11, iron=7 - выбираем gems
    Resource r = ds.collectBest();
    EXPECT_EQ(r, Resource::GEMS);
}

TEST(DungeonStateTest, QuestResourcePreferredWhenValueHigher) {
    // iron выигрывает
    auto ds = makeState(1, Resource::IRON, 5);
    ds.rooms[0].iron = 1;
    ds.rooms[0].gold = 1;
    ds.visit(0);

    Resource r = ds.collectBest();
    EXPECT_EQ(r, Resource::IRON); // 14 > 11
}

// финальное
TEST(DungeonStateTest, TotalValueWithQuestBonus) {
    auto ds = makeState(1, Resource::GEMS, 10);
    ds.collectedGold = 2;
    ds.collectedGems = 3;
    // gold: 2*11=22, gems: 3*46=138
    EXPECT_EQ(ds.totalValue(), 22 + 138);
}
