#include <gtest/gtest.h>
#include "core/GameData.h"
#include <unordered_set>

using namespace dod;

// ---- Races ----

TEST(GameData, SevenRacesExist) {
    EXPECT_EQ(GameData::getRaces().size(), 7u);
}

TEST(GameData, RaceNamesUnique) {
    std::unordered_set<std::string> names;
    for (const auto& r : GameData::getRaces()) {
        EXPECT_TRUE(names.insert(r.name).second)
            << "Duplicate race name: " << r.name;
    }
}

TEST(GameData, RaceBPCosts) {
    auto alv = GameData::findRace("Alv");
    ASSERT_TRUE(alv.has_value());
    EXPECT_EQ(alv->bpCost, 25);

    auto anka = GameData::findRace("Anka");
    ASSERT_TRUE(anka.has_value());
    EXPECT_EQ(anka->bpCost, 0);

    auto dvarg = GameData::findRace("Dvärg");
    ASSERT_TRUE(dvarg.has_value());
    EXPECT_EQ(dvarg->bpCost, 25);

    auto halvalv = GameData::findRace("Halvalv");
    ASSERT_TRUE(halvalv.has_value());
    EXPECT_EQ(halvalv->bpCost, 15);

    auto halvl = GameData::findRace("Halvlängdsman");
    ASSERT_TRUE(halvl.has_value());
    EXPECT_EQ(halvl->bpCost, 15);

    auto halvorch = GameData::findRace("Halvorch");
    ASSERT_TRUE(halvorch.has_value());
    EXPECT_EQ(halvorch->bpCost, 10);

    auto manniska = GameData::findRace("Människa");
    ASSERT_TRUE(manniska.has_value());
    EXPECT_EQ(manniska->bpCost, 10);
}

TEST(GameData, AlvStatMods) {
    auto alv = GameData::findRace("Alv");
    ASSERT_TRUE(alv.has_value());
    EXPECT_EQ(alv->statMods[STAT_STY], -1);
    EXPECT_EQ(alv->statMods[STAT_SMI], +3);
    EXPECT_EQ(alv->statMods[STAT_INT], +3);
    EXPECT_EQ(alv->statMods[STAT_KAR], +2);
    EXPECT_EQ(alv->statMods[STAT_FYS],  0);
    EXPECT_EQ(alv->statMods[STAT_PSY],  0);
}

TEST(GameData, AlvSTORange) {
    auto alv = GameData::findRace("Alv");
    ASSERT_TRUE(alv.has_value());
    EXPECT_EQ(alv->stoRange.minVal,  8);
    EXPECT_EQ(alv->stoRange.normal, 11);
    EXPECT_EQ(alv->stoRange.maxVal, 14);
}

TEST(GameData, AlvSpecialBonuses) {
    auto alv = GameData::findRace("Alv");
    ASSERT_TRUE(alv.has_value());
    EXPECT_GE(alv->specialBonuses.size(), 2u);
    bool hasDiscoverDanger = false, hasLyssna = false;
    for (const auto& b : alv->specialBonuses) {
        if (b.skillName == "Upptäcka fara" && b.fvBonus == 4 && !b.setFixed)
            hasDiscoverDanger = true;
        if (b.skillName == "Lyssna"        && b.fvBonus == 4 && !b.setFixed)
            hasLyssna = true;
    }
    EXPECT_TRUE(hasDiscoverDanger);
    EXPECT_TRUE(hasLyssna);
}

TEST(GameData, AnkaSpecialBonuses) {
    auto anka = GameData::findRace("Anka");
    ASSERT_TRUE(anka.has_value());
    bool hasSwim = false;
    for (const auto& b : anka->specialBonuses) {
        if (b.skillName == "Simma" && b.fvBonus == 20 && b.setFixed)
            hasSwim = true;
    }
    EXPECT_TRUE(hasSwim);
}

TEST(GameData, DvargSpecialBonus) {
    auto dvarg = GameData::findRace("Dvärg");
    ASSERT_TRUE(dvarg.has_value());
    bool hasGeo = false;
    for (const auto& b : dvarg->specialBonuses) {
        if (b.skillName == "Geologi" && b.fvBonus == 5 && b.setFixed)
            hasGeo = true;
    }
    EXPECT_TRUE(hasGeo);
}

// ---- Professions ----

TEST(GameData, ElevenProfessionsExist) {
    EXPECT_EQ(GameData::getProfessions().size(), 11u);
}

TEST(GameData, ProfessionNamesUnique) {
    std::unordered_set<std::string> names;
    for (const auto& p : GameData::getProfessions()) {
        EXPECT_TRUE(names.insert(p.name).second)
            << "Duplicate profession: " << p.name;
    }
}

TEST(GameData, MagikerHasNineSkillSlots) {
    auto m = GameData::findProfession("Magiker");
    ASSERT_TRUE(m.has_value());
    EXPECT_EQ(m->maxSkillChoices, 9);
}

TEST(GameData, AllOtherProfessionsHaveTwelveSkillSlots) {
    for (const auto& p : GameData::getProfessions()) {
        if (p.name != "Magiker") {
            EXPECT_EQ(p.maxSkillChoices, 12)
                << "Profession " << p.name << " should have 12 skill slots";
        }
    }
}

TEST(GameData, KrigareRequirements) {
    auto k = GameData::findProfession("Krigare");
    ASSERT_TRUE(k.has_value());
    EXPECT_EQ(k->statRequirements.at("STY"), 14);
    EXPECT_EQ(k->statRequirements.at("FYS"), 12);
}

TEST(GameData, RiddareRequirements) {
    auto r = GameData::findProfession("Riddare");
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r->statRequirements.at("STY"), 14);
    EXPECT_EQ(r->statRequirements.at("FYS"), 12);
    EXPECT_EQ(r->statRequirements.at("PSY"), 12);
}

TEST(GameData, LardManRequirements) {
    auto l = GameData::findProfession("Lärd man");
    ASSERT_TRUE(l.has_value());
    EXPECT_EQ(l->statRequirements.at("INT"), 16);
    EXPECT_EQ(l->statRequirements.size(), 1u);
}

// ---- Skills ----

TEST(GameData, NineteenPrimarySkillsExist) {
    EXPECT_EQ(GameData::getPrimarySkills().size(), 19u);
}

TEST(GameData, PrimarySkillsAllHaveBaseStat) {
    for (const auto& s : GameData::getPrimarySkills()) {
        EXPECT_FALSE(s.baseStat.empty())
            << "Primary skill " << s.name << " has no base stat";
        EXPECT_EQ(s.type, SkillType::PRIMARY);
    }
}

TEST(GameData, SkillNamesUnique) {
    std::unordered_set<std::string> names;
    for (const auto& s : GameData::getAllSkills()) {
        EXPECT_TRUE(names.insert(s.name).second)
            << "Duplicate skill: " << s.name;
    }
}

TEST(GameData, KnownPrimarySkillsPresent) {
    const std::vector<std::string> expected = {
        "Bluffa", "Finna dolda ting", "Första hjälpen", "Gömma sig",
        "Hoppa",  "Klättra", "Köpslå", "Lyssna",
        "Läsa/Skriva modersmål", "Rida", "Sjunga", "Slagsmål",
        "Smyga",  "Spåra", "Stjäla föremål", "Tala modersmål",
        "Upptäcka fara", "Värdera", "Övertala"
    };
    for (const auto& name : expected) {
        auto found = GameData::findSkill(name);
        EXPECT_TRUE(found.has_value()) << "Missing primary skill: " << name;
        if (found) { EXPECT_EQ(found->type, SkillType::PRIMARY) << name; }
    }
}

TEST(GameData, SecondarySkillsExist) {
    EXPECT_GT(GameData::getSecondarySkills().size(), 0u);
}

TEST(GameData, FindRaceReturnsNulloptForUnknown) {
    auto r = GameData::findRace("UnknownRace");
    EXPECT_FALSE(r.has_value());
}

TEST(GameData, FindProfessionReturnsNulloptForUnknown) {
    auto p = GameData::findProfession("NonExistentProfession");
    EXPECT_FALSE(p.has_value());
}

TEST(GameData, FindSkillReturnsNulloptForUnknown) {
    auto s = GameData::findSkill("FlyingSkill");
    EXPECT_FALSE(s.has_value());
}

// ---- Age categories ----

TEST(GameData, FourAgeCategoriesExist) {
    EXPECT_EQ(GameData::getAgeCategories().size(), 4u);
}

TEST(GameData, AgeCategoryEPIncreases) {
    const auto& ages = GameData::getAgeCategories();
    for (std::size_t i = 1; i < ages.size(); ++i) {
        EXPECT_GE(ages[i].ep, ages[i-1].ep)
            << "EP should not decrease from " << ages[i-1].name
            << " to " << ages[i].name;
    }
}

TEST(GameData, YoungCategoryValues) {
    const auto& ages = GameData::getAgeCategories();
    // Young is first
    EXPECT_EQ(ages[0].ep,          150);
    EXPECT_DOUBLE_EQ(ages[0].capitalMultiplier, 1.0);
    EXPECT_EQ(ages[0].maxStartFV,  13);
}

// ---- Social standing table ----

TEST(GameData, SocialStandingTableNonEmpty) {
    EXPECT_GT(GameData::getSocialStandingTable().size(), 0u);
}

TEST(GameData, SocialStandingTableCoversRoll2) {
    bool found = false;
    for (const auto& e : GameData::getSocialStandingTable()) {
        if (e.rollMin <= 2 && e.rollMax >= 2) { found = true; break; }
    }
    EXPECT_TRUE(found);
}
