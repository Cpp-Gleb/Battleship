#include <gtest/gtest.h>
#include "Game/MasterGame.h"
#include "Game/SlaveGame.h"

TEST(GameTest, MasterGameCreateAndSet) {
    MasterGame mg;
    ASSERT_TRUE(mg.TryCreate(PlayerRole::Master));
    ASSERT_TRUE(mg.TrySetWidth(10));
    ASSERT_TRUE(mg.TrySetHeight(10));
    ASSERT_TRUE(mg.TrySetCount(1, 2));
    ASSERT_TRUE(mg.TryStart());

    ASSERT_EQ(mg.GetWidth(), 10ULL);
    ASSERT_EQ(mg.GetHeight(), 10ULL);
    ASSERT_EQ(mg.GetCount(1), 2ULL);
    EXPECT_EQ(mg.Finished(), "no");
}

TEST(GameTest, SlaveGameCreateAndLoad) {
    SlaveGame sg;
    ASSERT_TRUE(sg.TryCreate(PlayerRole::Slave));
}
