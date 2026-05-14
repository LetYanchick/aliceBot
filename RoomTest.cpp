#include <gtest/gtest.h>
#include "../Room.h"

TEST(ResourceTest, NamesRoundTrip) {
    EXPECT_EQ(resourceFromString(resourceName(Resource::IRON)), Resource::IRON);
    EXPECT_EQ(resourceFromString(resourceName(Resource::GOLD)), Resource::GOLD);
    EXPECT_EQ(resourceFromString(resourceName(Resource::GEMS)), Resource::GEMS);
    EXPECT_EQ(resourceFromString(resourceName(Resource::EXP)),  Resource::EXP);
}

TEST(ResourceTest, UnknownStringReturnsNone) {
    EXPECT_EQ(resourceFromString("diamond"), Resource::NONE);
    EXPECT_EQ(resourceFromString(""),        Resource::NONE);
}

TEST(ResourceTest, BaseValues) {
    EXPECT_EQ(resourceBaseValue(Resource::IRON), 7);
    EXPECT_EQ(resourceBaseValue(Resource::GOLD), 11);
    EXPECT_EQ(resourceBaseValue(Resource::GEMS), 23);
    EXPECT_EQ(resourceBaseValue(Resource::EXP),  1);
}

TEST(RoomTest, ResourceRefAndCount) {
    Room r;
    r.iron = 3; r.gold = 0; r.gems = 5; r.exp = 1;

    EXPECT_EQ(r.resourceCount(Resource::IRON), 3);
    EXPECT_EQ(r.resourceCount(Resource::GOLD), 0);
    EXPECT_EQ(r.resourceCount(Resource::GEMS), 5);

    r.resourceRef(Resource::IRON) = 0;
    EXPECT_EQ(r.resourceCount(Resource::IRON), 0);
}
