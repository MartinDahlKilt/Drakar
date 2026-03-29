#include <gtest/gtest.h>
#include "core/CharacterSerializer.h"
#include "core/Character.h"
#include "core/SkillId.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#  include <windows.h>
#endif

using namespace dod;
using json = nlohmann::json;

// Returns a writable temp directory path (no trailing separator).
static std::string getTempDir() {
#ifdef _WIN32
    char buf[MAX_PATH];
    DWORD len = GetTempPathA(MAX_PATH, buf);
    if (len > 0 && len < MAX_PATH) {
        std::string s(buf, len);
        while (!s.empty() && (s.back() == '\\' || s.back() == '/')) s.pop_back();
        return s;
    }
    return ".";
#else
    const char* tmp = std::getenv("TMPDIR");
    if (!tmp) tmp = "/tmp";
    return tmp;
#endif
}

// Helper: construct a minimal but valid Character
static Character makeTestCharacter() {
    Character c;
    c.name       = "Test Hero";
    c.gender     = "Male";
    c.race       = "Människa";
    c.profession = "Krigare";
    c.ageCategory = "Mature (Mogen)";

    c.purchasedStats = {14, 12, 10, 8, 9, 8, 12};
    c.raceMods       = {0, 0, 0, 0, 0, 0, 0};
    c.ageMods        = {0, 0, 0, 0, 0, 0, 0};
    c.finalStats     = {14, 12, 10, 8, 9, 8, 12};

    c.derived.hpTotal     = 12;
    c.derived.hpParts     = {6, 4, 3, 3, 4, 4};
    c.derived.damageBonus = "none";
    c.derived.movement    = 10;

    c.bpTotal     = 125;
    c.bpSpent     = 80;
    c.bpRemaining = 45;

    c.hasSpecialAbility     = false;
    c.specialAbilityBPSpent = 0;
    c.specialAbilityDesc    = "";

    c.socialRoll        = 8;
    c.socialDescription = "Lower middle class";
    c.startingCapital   = 500;
    c.weaponHandRoll    = 9;
    c.weaponHand        = "Right";

    SkillEntry sword;
    sword.name              = "Weapon: Sword";
    sword.baseStat          = "SMI";
    sword.fv                = 2;
    sword.isPrimary         = false;
    sword.isProfessionSkill = true;
    c.skills.push_back(sword);

    SkillEntry hoppa;
    hoppa.name      = "Hoppa";
    hoppa.baseStat  = "SMI";
    hoppa.fv        = 2;
    hoppa.isPrimary = true;
    hoppa.isProfessionSkill = false;
    c.skills.push_back(hoppa);

    c.epFromAge = 200;
    c.epFromBP  = 225;
    c.epTotal   = 425;

    c.appearance  = "Tall with a scar";
    c.personality = "Stoic";
    c.background  = "Former soldier";

    RollRecord rr;
    rr.context   = "Weapon hand";
    rr.diceExpr  = "2T6+0";
    rr.rolls     = {4, 5};
    rr.modifier  = 0;
    rr.total     = 9;
    rr.outcome   = "Right";
    c.rollHistory.push_back(rr);

    return c;
}

// ---- toJson: required top-level keys ----

TEST(CharacterSerializer, JsonHasRequiredTopLevelKeys) {
    auto j = CharacterSerializer::toJson(makeTestCharacter());

    EXPECT_TRUE(j.contains("name"));
    EXPECT_TRUE(j.contains("gender"));
    EXPECT_TRUE(j.contains("race"));
    EXPECT_TRUE(j.contains("profession"));
    EXPECT_TRUE(j.contains("age_category"));
    EXPECT_TRUE(j.contains("stats"));
    EXPECT_TRUE(j.contains("derived"));
    EXPECT_TRUE(j.contains("background_points"));
    EXPECT_TRUE(j.contains("special_ability"));
    EXPECT_TRUE(j.contains("social"));
    EXPECT_TRUE(j.contains("weapon_hand"));
    EXPECT_TRUE(j.contains("experience"));
    EXPECT_TRUE(j.contains("skills"));
    EXPECT_TRUE(j.contains("roll_history"));
    EXPECT_TRUE(j.contains("appearance"));
    EXPECT_TRUE(j.contains("personality"));
    EXPECT_TRUE(j.contains("background"));
}

// ---- toJson: stats nested keys ----

TEST(CharacterSerializer, JsonStatsHasSubKeys) {
    auto j = CharacterSerializer::toJson(makeTestCharacter());
    const auto& stats = j["stats"];

    EXPECT_TRUE(stats.contains("purchased"));
    EXPECT_TRUE(stats.contains("race_mods"));
    EXPECT_TRUE(stats.contains("age_mods"));
    EXPECT_TRUE(stats.contains("final"));

    for (const char* key : {"STY","FYS","SMI","INT","PSY","KAR","STO"}) {
        EXPECT_TRUE(stats["final"].contains(key)) << "Missing stat: " << key;
    }
}

// ---- toJson: derived keys ----

TEST(CharacterSerializer, JsonDerivedHasSubKeys) {
    auto j = CharacterSerializer::toJson(makeTestCharacter());
    const auto& d = j["derived"];

    EXPECT_TRUE(d.contains("hp_total"));
    EXPECT_TRUE(d.contains("hp_per_body_part"));
    EXPECT_TRUE(d.contains("damage_bonus"));
    EXPECT_TRUE(d.contains("movement"));
}

// ---- toJson: correct values ----

TEST(CharacterSerializer, JsonValuesFidelity) {
    Character c = makeTestCharacter();
    auto j = CharacterSerializer::toJson(c);

    EXPECT_EQ(j["name"].get<std::string>(), "Test Hero");
    EXPECT_EQ(j["race"].get<std::string>(), "Människa");
    EXPECT_EQ(j["profession"].get<std::string>(), "Krigare");
    EXPECT_EQ(j["stats"]["final"]["STY"].get<int>(), 14);
    EXPECT_EQ(j["stats"]["final"]["FYS"].get<int>(), 12);
    EXPECT_EQ(j["derived"]["hp_total"].get<int>(), 12);
    EXPECT_EQ(j["derived"]["damage_bonus"].get<std::string>(), "none");
    EXPECT_EQ(j["social"]["starting_capital_sm"].get<int>(), 500);
    EXPECT_EQ(j["weapon_hand"]["hand"].get<std::string>(), "Right");
    EXPECT_EQ(j["experience"]["ep_total"].get<int>(), 425);
    EXPECT_EQ(j["appearance"].get<std::string>(), "Tall with a scar");
}

// ---- toJson: skills array ----

TEST(CharacterSerializer, JsonSkillsArray) {
    auto j = CharacterSerializer::toJson(makeTestCharacter());
    ASSERT_TRUE(j["skills"].is_array());
    ASSERT_EQ(j["skills"].size(), 2u);

    const auto& firstSkill = j["skills"][0];
    EXPECT_TRUE(firstSkill.contains("name"));
    EXPECT_TRUE(firstSkill.contains("base_stat"));
    EXPECT_TRUE(firstSkill.contains("fv"));
    EXPECT_TRUE(firstSkill.contains("is_primary"));
    EXPECT_TRUE(firstSkill.contains("is_profession_skill"));
}

// ---- toJson: roll history ----

TEST(CharacterSerializer, JsonRollHistory) {
    auto j = CharacterSerializer::toJson(makeTestCharacter());
    ASSERT_TRUE(j["roll_history"].is_array());
    ASSERT_EQ(j["roll_history"].size(), 1u);

    const auto& roll = j["roll_history"][0];
    EXPECT_EQ(roll["context"].get<std::string>(), "Weapon hand");
    EXPECT_EQ(roll["total"].get<int>(), 9);
    EXPECT_EQ(roll["outcome"].get<std::string>(), "Right");
}

// ---- saveToFile ----

TEST(CharacterSerializer, SaveToFileCreatesFile) {
    Character c = makeTestCharacter();
    const std::string path = getTempDir() + "/dod_test_character.json";

    EXPECT_NO_THROW(CharacterSerializer::saveToFile(c, path));

    // Read back and check top-level key
    std::ifstream f(path);
    ASSERT_TRUE(f.is_open());
    json loaded;
    f >> loaded;
    EXPECT_EQ(loaded["name"].get<std::string>(), "Test Hero");
}

TEST(CharacterSerializer, SaveToInvalidPathThrows) {
    Character c = makeTestCharacter();
    // Use a path whose parent directory cannot exist on any platform
    EXPECT_THROW(
        CharacterSerializer::saveToFile(c, getTempDir() + "/dod_nonexistent_dir_xyz/file.json"),
        std::runtime_error
    );
}

// ---- EP and SA grant fields ----

TEST(CharacterSerializer, SkillsHaveFvBaseAndClNote) {
    auto j = CharacterSerializer::toJson(makeTestCharacter());
    ASSERT_TRUE(j["skills"].is_array());
    ASSERT_FALSE(j["skills"].empty());
    const auto& s0 = j["skills"][0];
    EXPECT_TRUE(s0.contains("fv_base"));
    EXPECT_TRUE(s0.contains("cl_note"));
    EXPECT_EQ(s0["fv_base"].get<int>(), 0);
    EXPECT_EQ(s0["cl_note"].get<std::string>(), "");
}

TEST(CharacterSerializer, ExperienceHasEpSpent) {
    auto j = CharacterSerializer::toJson(makeTestCharacter());
    ASSERT_TRUE(j["experience"].contains("ep_spent"));
    EXPECT_EQ(j["experience"]["ep_spent"].get<int>(), 0);
}

TEST(CharacterSerializer, SpecialAbilityGrantsIsArray) {
    auto j = CharacterSerializer::toJson(makeTestCharacter());
    ASSERT_TRUE(j.contains("special_ability_grants"));
    EXPECT_TRUE(j["special_ability_grants"].is_array());
    EXPECT_TRUE(j["special_ability_grants"].empty());
}

TEST(CharacterSerializer, SpecialAbilityGrantsRoundTrip) {
    Character c = makeTestCharacter();
    SpecialAbilityGrant g;
    g.type        = SpecialAbilityGrant::Type::FV;
    g.skillId     = SkillId::HOPPA;
    g.amount      = 3;
    g.playerChoice = false;
    c.specialAbilityGrants.push_back(g);

    auto j = CharacterSerializer::toJson(c);
    ASSERT_EQ(j["special_ability_grants"].size(), 1u);
    const auto& entry = j["special_ability_grants"][0];
    EXPECT_EQ(entry["type"].get<std::string>(),      "FV");
    EXPECT_EQ(entry["skill_id"].get<std::string>(),  "HOPPA");
    EXPECT_EQ(entry["skill_name"].get<std::string>(), "Hoppa");
    EXPECT_EQ(entry["amount"].get<int>(),             3);
    EXPECT_EQ(entry["player_choice"].get<bool>(),     false);
}
