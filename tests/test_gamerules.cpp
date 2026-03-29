#include <gtest/gtest.h>
#include "core/GameRules.h"
#include "core/GameData.h"

using namespace dod;
using namespace dod::GameRules;

// ---- getStatBPCost ----

TEST(GameRules, StatBPCostKnownValues) {
    EXPECT_EQ(getStatBPCost(3),  0);
    EXPECT_EQ(getStatBPCost(4),  1);
    EXPECT_EQ(getStatBPCost(5),  2);
    EXPECT_EQ(getStatBPCost(6),  3);
    EXPECT_EQ(getStatBPCost(7),  5);
    EXPECT_EQ(getStatBPCost(8),  7);
    EXPECT_EQ(getStatBPCost(9),  9);
    EXPECT_EQ(getStatBPCost(10), 10);
    EXPECT_EQ(getStatBPCost(11), 11);
    EXPECT_EQ(getStatBPCost(12), 12);
    EXPECT_EQ(getStatBPCost(13), 14);
    EXPECT_EQ(getStatBPCost(14), 17);
    EXPECT_EQ(getStatBPCost(15), 20);
    EXPECT_EQ(getStatBPCost(16), 25);
    EXPECT_EQ(getStatBPCost(17), 30);
    EXPECT_EQ(getStatBPCost(18), 40);
}

TEST(GameRules, StatBPCostOutOfRangeThrows) {
    EXPECT_THROW(getStatBPCost(2),  std::out_of_range);
    EXPECT_THROW(getStatBPCost(19), std::out_of_range);
    EXPECT_THROW(getStatBPCost(0),  std::out_of_range);
}

TEST(GameRules, StatRaiseCostIsMarginDifference) {
    EXPECT_EQ(getStatRaiseCost(3,  5), getStatBPCost(5) - getStatBPCost(3));
    EXPECT_EQ(getStatRaiseCost(10, 14), getStatBPCost(14) - getStatBPCost(10));
    EXPECT_EQ(getStatRaiseCost(3,  3), 0);
}

// ---- calculateBC ----

TEST(GameRules, BCAtBoundaries) {
    EXPECT_EQ(calculateBC(1),  0);
    EXPECT_EQ(calculateBC(3),  0);
    EXPECT_EQ(calculateBC(4),  1);
    EXPECT_EQ(calculateBC(8),  1);
    EXPECT_EQ(calculateBC(9),  2);
    EXPECT_EQ(calculateBC(12), 2);
    EXPECT_EQ(calculateBC(13), 3);
    EXPECT_EQ(calculateBC(16), 3);
    EXPECT_EQ(calculateBC(17), 4);
    EXPECT_EQ(calculateBC(20), 4);
    EXPECT_EQ(calculateBC(21), 5);
    EXPECT_EQ(calculateBC(30), 5);
}

// ---- calculateHP ----

TEST(GameRules, HPCalculationCeilsHalf) {
    EXPECT_EQ(calculateHP(10, 10), 10);   // even
    EXPECT_EQ(calculateHP(10, 11), 11);   // ceil(21/2)=11
    EXPECT_EQ(calculateHP(12, 13), 13);   // ceil(25/2)=13
    EXPECT_EQ(calculateHP(3,  3),  3);    // ceil(6/2)=3
    EXPECT_EQ(calculateHP(5,  4),  5);    // ceil(9/2)=5
}

// ---- calculateBodyPartHP ----

TEST(GameRules, BodyPartHPNonZeroForPositiveTotal) {
    BodyPartHP hp = calculateBodyPartHP(10);
    EXPECT_GT(hp.brostKorg,  0);
    EXPECT_GT(hp.huvud,      0);
    EXPECT_GT(hp.vansterArm, 0);
    EXPECT_GT(hp.hogerArm,   0);
    EXPECT_GT(hp.vansterBen, 0);
    EXPECT_GT(hp.hogerBen,   0);
}

TEST(GameRules, BodyPartHPArmsEqual) {
    BodyPartHP hp = calculateBodyPartHP(12);
    EXPECT_EQ(hp.vansterArm, hp.hogerArm);
    EXPECT_EQ(hp.vansterBen, hp.hogerBen);
}

TEST(GameRules, BodyPartHPChestLargest) {
    BodyPartHP hp = calculateBodyPartHP(8);
    EXPECT_GE(hp.brostKorg, hp.huvud);
    EXPECT_GE(hp.brostKorg, hp.vansterArm);
}

// ---- calculateDamageBonus ----

TEST(GameRules, DamageBonusAtBoundaries) {
    EXPECT_EQ(calculateDamageBonus(13, 13), "none");  // sum 26
    EXPECT_EQ(calculateDamageBonus(14, 13), "+1T2");  // sum 27
    EXPECT_EQ(calculateDamageBonus(15, 14), "+1T2");  // sum 29
    EXPECT_EQ(calculateDamageBonus(15, 15), "+1T4");  // sum 30
    EXPECT_EQ(calculateDamageBonus(16, 16), "+1T4");  // sum 32
    EXPECT_EQ(calculateDamageBonus(17, 16), "+1T6");  // sum 33
    EXPECT_EQ(calculateDamageBonus(20, 20), "+1T6");  // sum 40
    EXPECT_EQ(calculateDamageBonus(21, 20), "+2T6");  // sum 41
    EXPECT_EQ(calculateDamageBonus(25, 25), "+2T6");  // sum 50
    EXPECT_EQ(calculateDamageBonus(26, 25), "+3T6");  // sum 51
    EXPECT_EQ(calculateDamageBonus(30, 30), "+3T6");  // sum 60
    EXPECT_EQ(calculateDamageBonus(31, 30), "+4T6");  // sum 61
}

// ---- calculateMovement ----

TEST(GameRules, MovementIsPositive) {
    EXPECT_GT(calculateMovement(10, 10, 10, "Människa"), 0);
}

TEST(GameRules, AnkaMovementLower) {
    int human = calculateMovement(13, 12, 12, "Människa");
    int anka  = calculateMovement(5,  12, 12, "Anka");
    // Anka has lower STO, and -2 race penalty
    EXPECT_LT(anka, human);
}

// ---- Social standing ----

TEST(GameRules, SocialStandingDescKnown) {
    // Roll 2 = propertyless tier
    std::string desc2 = getSocialStandingDesc(2);
    EXPECT_FALSE(desc2.empty());

    // Roll 8 = lower middle class
    std::string desc8 = getSocialStandingDesc(8);
    EXPECT_FALSE(desc8.empty());

    // Roll 2 and 8 should be different
    EXPECT_NE(desc2, desc8);
}

TEST(GameRules, StartingCapitalBaseIncreases) {
    int cap2  = getStartingCapitalBase(2);
    int cap12 = getStartingCapitalBase(12);
    int cap30 = getStartingCapitalBase(30);
    EXPECT_LT(cap2,  cap12);
    EXPECT_LT(cap12, cap30);
}

TEST(GameRules, StartingCapitalWithAgeMultiplier) {
    int base     = 500;
    int result   = calculateStartingCapital(base, 2.0);
    EXPECT_EQ(result, 1000);
}

// ---- meetsStatRequirements ----

TEST(GameRules, KrigareRequirementsPass) {
    auto profOpt = GameData::findProfession("Krigare");
    ASSERT_TRUE(profOpt.has_value());

    Stats s{};
    s.STY = 14; s.FYS = 12;
    EXPECT_TRUE(meetsStatRequirements(s, *profOpt));
}

TEST(GameRules, KrigareRequirementsFail) {
    auto profOpt = GameData::findProfession("Krigare");
    ASSERT_TRUE(profOpt.has_value());

    Stats s{};
    s.STY = 13; s.FYS = 12;  // STY too low
    EXPECT_FALSE(meetsStatRequirements(s, *profOpt));
}

TEST(GameRules, MagikerRequirements) {
    auto profOpt = GameData::findProfession("Magiker");
    ASSERT_TRUE(profOpt.has_value());

    Stats pass{};
    pass.INT = 12; pass.PSY = 14;
    EXPECT_TRUE(meetsStatRequirements(pass, *profOpt));

    Stats fail{};
    fail.INT = 12; fail.PSY = 13;
    EXPECT_FALSE(meetsStatRequirements(fail, *profOpt));
}

TEST(GameRules, TjuvRequirements) {
    auto profOpt = GameData::findProfession("Tjuv");
    ASSERT_TRUE(profOpt.has_value());

    Stats pass{}; pass.SMI = 16;
    EXPECT_TRUE(meetsStatRequirements(pass, *profOpt));

    Stats fail{}; fail.SMI = 15;
    EXPECT_FALSE(meetsStatRequirements(fail, *profOpt));
}

TEST(GameRules, ManniskaNoRequirements) {
    // Any profession with no stat requirements should pass with all-zero stats
    // (Människa profession requirements: none → there is no "Människa profession",
    //  but we test a real profession with no hard requirements)
    // Use Bard but with stats meeting its min (SMI12, KAR14)
    auto profOpt = GameData::findProfession("Bard");
    ASSERT_TRUE(profOpt.has_value());

    Stats s{}; s.SMI = 12; s.KAR = 14;
    EXPECT_TRUE(meetsStatRequirements(s, *profOpt));

    Stats sf{}; sf.SMI = 11; sf.KAR = 14;
    EXPECT_FALSE(meetsStatRequirements(sf, *profOpt));
}

// ---- STO adjustment costs ----

TEST(GameRules, STOAdjustmentCost) {
    EXPECT_EQ(getSTOAdjustmentCost(0),  0);
    EXPECT_EQ(getSTOAdjustmentCost(1),  2);
    EXPECT_EQ(getSTOAdjustmentCost(-1), 2);
    EXPECT_EQ(getSTOAdjustmentCost(3),  6);
    EXPECT_EQ(getSTOAdjustmentCost(-3), 6);
}

// ---- EP cost functions ----

TEST(GameRules, EPCostMultiplierBands) {
    EXPECT_EQ(getEPCostMultiplier(1),  1);
    EXPECT_EQ(getEPCostMultiplier(10), 1);
    EXPECT_EQ(getEPCostMultiplier(11), 2);
    EXPECT_EQ(getEPCostMultiplier(14), 2);
    EXPECT_EQ(getEPCostMultiplier(15), 3);
    EXPECT_EQ(getEPCostMultiplier(17), 3);
    EXPECT_EQ(getEPCostMultiplier(18), 4);
    EXPECT_EQ(getEPCostMultiplier(20), 4);
    EXPECT_EQ(getEPCostMultiplier(21), 5);
    EXPECT_EQ(getEPCostMultiplier(30), 5);
}

TEST(GameRules, EPStepCostPrimary) {
    // Band 1 (fv<=10): base 2 * mult 1 = 2
    EXPECT_EQ(getEPStepCost(5,  true, false), 2);
    EXPECT_EQ(getEPStepCost(10, true, false), 2);
    // Band 2 (11-14): base 2 * mult 2 = 4
    EXPECT_EQ(getEPStepCost(12, true, false), 4);
    // Band 3 (15-17): base 2 * mult 3 = 6
    EXPECT_EQ(getEPStepCost(16, true, false), 6);
}

TEST(GameRules, EPStepCostProfession) {
    // Band 1: base 3 * mult 1 = 3
    EXPECT_EQ(getEPStepCost(5,  false, true), 3);
    // Band 2: base 3 * mult 2 = 6
    EXPECT_EQ(getEPStepCost(13, false, true), 6);
}

TEST(GameRules, EPStepCostSecondary) {
    // Band 1: base 5 * mult 1 = 5
    EXPECT_EQ(getEPStepCost(3,  false, false), 5);
    // Band 2: base 5 * mult 2 = 10
    EXPECT_EQ(getEPStepCost(11, false, false), 10);
}

TEST(GameRules, CalculateEPCostTotal) {
    // Primary: FV 9->12
    // step 10 = 2*1=2, step 11 = 2*2=4, step 12 = 2*2=4 => 10
    EXPECT_EQ(calculateEPCost(9, 12, true, false), 10);
    // Same start==end => 0
    EXPECT_EQ(calculateEPCost(5, 5, true, false), 0);
    // Secondary: FV 3->4 => 5*1=5
    EXPECT_EQ(calculateEPCost(3, 4, false, false), 5);
    // Profession: FV 10->11 => 3*2=6
    EXPECT_EQ(calculateEPCost(10, 11, false, true), 6);
}

TEST(GameRules, AgeFVCap) {
    EXPECT_EQ(getAgeFVCap("Young (Ung)"),              13);
    EXPECT_EQ(getAgeFVCap("Mature (Mogen)"),           15);
    EXPECT_EQ(getAgeFVCap("Mid-aged (Medelålders)"),   17);
    EXPECT_EQ(getAgeFVCap("Old (Gammal)"),             19);
    EXPECT_EQ(getAgeFVCap("Unknown"),                  20);
}
