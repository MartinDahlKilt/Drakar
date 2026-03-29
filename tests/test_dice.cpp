#include <gtest/gtest.h>
#include "core/Dice.h"

using namespace dod;

// ---- DiceResult::describe ----

TEST(DiceResult, DescribePositiveModifier) {
    DiceResult r;
    r.count    = 2;
    r.sides    = 6;
    r.modifier = 2;
    r.rolls    = {3, 5};
    r.total    = 10;
    EXPECT_EQ(r.describe(), "2T6+2: [3, 5] = 10");
}

TEST(DiceResult, DescribeNoModifier) {
    DiceResult r;
    r.count    = 1;
    r.sides    = 20;
    r.modifier = 0;
    r.rolls    = {15};
    r.total    = 15;
    EXPECT_EQ(r.describe(), "1T20: [15] = 15");
}

TEST(DiceResult, DescribeNegativeModifier) {
    DiceResult r;
    r.count    = 1;
    r.sides    = 6;
    r.modifier = -2;
    r.rolls    = {4};
    r.total    = 2;
    EXPECT_EQ(r.describe(), "1T6-2: [4] = 2");
}

// ---- DiceRoller::roll ----

TEST(DiceRoller, SeededRollIsReproducible) {
    DiceRoller a(42), b(42);
    EXPECT_EQ(a.roll(2, 6).total, b.roll(2, 6).total);
}

TEST(DiceRoller, RollD6InRange) {
    DiceRoller roller(1234);
    for (int i = 0; i < 200; ++i) {
        auto r = roller.roll(1, 6);
        EXPECT_GE(r.total, 1);
        EXPECT_LE(r.total, 6);
    }
}

TEST(DiceRoller, RollD20InRange) {
    DiceRoller roller(999);
    for (int i = 0; i < 200; ++i) {
        auto r = roller.roll(1, 20);
        EXPECT_GE(r.total, 1);
        EXPECT_LE(r.total, 20);
    }
}

TEST(DiceRoller, Roll2D6PlusModifier) {
    DiceRoller roller(7);
    for (int i = 0; i < 100; ++i) {
        auto r = roller.roll(2, 6, 3);
        EXPECT_GE(r.total, 5);   // 2+3
        EXPECT_LE(r.total, 15);  // 12+3
        EXPECT_EQ(static_cast<int>(r.rolls.size()), 2);
        EXPECT_EQ(r.modifier, 3);
    }
}

TEST(DiceRoller, RollCountMatchesDice) {
    DiceRoller roller(5);
    auto r = roller.roll(4, 8);
    EXPECT_EQ(static_cast<int>(r.rolls.size()), 4);
}

TEST(DiceRoller, TotalMatchesSumPlusModifier) {
    DiceRoller roller(123);
    auto r = roller.roll(3, 6, -1);
    int expected = -1;
    for (int v : r.rolls) expected += v;
    EXPECT_EQ(r.total, expected);
}

TEST(DiceRoller, InvalidCountThrows) {
    DiceRoller roller;
    EXPECT_THROW(roller.roll(0, 6), std::invalid_argument);
    EXPECT_THROW(roller.roll(-1, 6), std::invalid_argument);
}

TEST(DiceRoller, InvalidSidesThrow) {
    DiceRoller roller;
    EXPECT_THROW(roller.roll(1, 1), std::invalid_argument);
    EXPECT_THROW(roller.roll(1, 0), std::invalid_argument);
}

// ---- T3 ----

TEST(DiceRoller, RollT3InRange) {
    DiceRoller roller(77);
    for (int i = 0; i < 200; ++i) {
        auto r = roller.rollT3();
        EXPECT_GE(r.total, 1);
        EXPECT_LE(r.total, 3);
        EXPECT_EQ(r.sides, 3);
    }
}

TEST(DiceRoller, RollT3BothFaces) {
    DiceRoller roller(0);
    bool saw1 = false, saw2 = false, saw3 = false;
    for (int i = 0; i < 300; ++i) {
        int v = roller.rollT3().total;
        if (v == 1) saw1 = true;
        if (v == 2) saw2 = true;
        if (v == 3) saw3 = true;
    }
    EXPECT_TRUE(saw1);
    EXPECT_TRUE(saw2);
    EXPECT_TRUE(saw3);
}

// ---- T2 ----

TEST(DiceRoller, RollT2InRange) {
    DiceRoller roller(55);
    for (int i = 0; i < 200; ++i) {
        auto r = roller.rollT2();
        EXPECT_GE(r.total, 1);
        EXPECT_LE(r.total, 2);
    }
}

// ---- Percent ----

TEST(DiceRoller, RollPercentInRange) {
    DiceRoller roller(321);
    for (int i = 0; i < 200; ++i) {
        auto r = roller.rollPercent();
        EXPECT_GE(r.total, 1);
        EXPECT_LE(r.total, 100);
    }
}
