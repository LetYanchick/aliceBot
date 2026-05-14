#include <gtest/gtest.h>
#include "TestHelpers.h"
#include "Pathfinder.h"

TEST(PathfinderTest, ShortestPathLinear) {
    // 0 - 1 - 2 - 3, all visited
    auto ds = makeState(3, Resource::GOLD, 10);
    link(ds, 0, 1); link(ds, 1, 2); link(ds, 2, 3);
    ds.visit(0); ds.visit(1); ds.visit(2); ds.visit(3);

    auto path = Pathfinder::shortestVisited(ds, 0, 3);
    ASSERT_EQ(path.size(), 3u);
    EXPECT_EQ(path[0], 1);
    EXPECT_EQ(path[1], 2);
    EXPECT_EQ(path[2], 3);
}

TEST(PathfinderTest, TieBreaksBySmallestId) {
    //   1
    //  / \
    // 0   3
    //  \ /
    //   2
    auto ds = makeState(3, Resource::GOLD, 10);
    link(ds, 0, 1); link(ds, 0, 2);
    link(ds, 1, 3); link(ds, 2, 3);
    ds.visit(0); ds.visit(1); ds.visit(2); ds.visit(3);

    auto path = Pathfinder::shortestVisited(ds, 0, 3);
    ASSERT_EQ(path.size(), 2u);
    EXPECT_EQ(path[0], 1); // выбирает 1
}

TEST(PathfinderTest, UnreachableReturnsEmpty) {
    auto ds = makeState(2, Resource::GOLD, 10);
    link(ds, 0, 1);
    // до 2 комнаты не добраться
    ds.visit(0); ds.visit(1); ds.visit(2);

    auto path = Pathfinder::shortestVisited(ds, 0, 2);
    EXPECT_TRUE(path.empty());
}

TEST(PathfinderTest, UnvisitedRoomNotUsed) {
    // 0 - 1 - 2 - 3, но без 2
    auto ds = makeState(3, Resource::GOLD, 10);
    link(ds, 0, 1); link(ds, 1, 2); link(ds, 2, 3);
    ds.visit(0); ds.visit(1); ds.visit(3); // 2 не посещена

    auto path = Pathfinder::shortestVisited(ds, 0, 3);
    EXPECT_TRUE(path.empty()); // не добраться до 3 без 2
}

TEST(PathfinderTest, SameRoomReturnsEmptyPath) {
    auto ds = makeState(1, Resource::GOLD, 10);
    ds.visit(0);

    auto path = Pathfinder::shortestVisited(ds, 0, 0);
    EXPECT_TRUE(path.empty());
    EXPECT_EQ(Pathfinder::distanceVisited(ds, 0, 0), 0);
}
