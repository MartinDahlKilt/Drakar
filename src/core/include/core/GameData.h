#pragma once
#include "core/Character.h"
#include "core/SkillId.h"
#include <string>
#include <vector>
#include <map>
#include <optional>

namespace dod {

// ---- Stat indices (used as array index into Stats arrays) ----
enum StatIndex : int {
    STAT_STY = 0,
    STAT_FYS = 1,
    STAT_SMI = 2,
    STAT_INT = 3,
    STAT_PSY = 4,
    STAT_KAR = 5,
    STAT_STO = 6,
    STAT_COUNT = 7
};

inline const char* statName(StatIndex idx) {
    static const char* names[] = {"STY","FYS","SMI","INT","PSY","KAR","STO"};
    return names[static_cast<int>(idx)];
}

// ---- Skill type ----
enum class SkillType { PRIMARY, SECONDARY };

// ---- Special bonus granted by race ----
struct RaceSkillBonus {
    SkillId     skillId;
    int         fvBonus;      ///< Added to the skill's BC at creation
    bool        setFixed;     ///< If true, the FV is set to fvBonus (not added)
};

// ---- STO range for a race ----
struct StoRange {
    int minVal;
    int normal;
    int maxVal;
};

// ---- Race definition ----
struct RaceDefinition {
    std::string name;
    int         bpCost;
    int         statMods[STAT_COUNT];  ///< Index = StatIndex; applied after BP purchase
    StoRange    stoRange;
    std::vector<RaceSkillBonus> specialBonuses;
    std::string description;
};

// ---- Skill definition ----
struct SkillDefinition {
    SkillId     id;
    SkillType   type;
    std::string baseStat;      ///< e.g. "KAR", "INT", "SMI", "STY", "PSY", "var."
    bool        isWeapon     = false;
    bool        isLanguage   = false;  ///< spoken language
    bool        isLiteracy   = false;  ///< read/write
    bool        isInstrument = false;
    bool        isCraft      = false;
    bool        isMagicSchool = false;
};

// ---- Weapon-skill limit within a profession ----
struct WeaponLimit {
    int maxCount;          ///< Max separate weapon skills choosable; -1 = unlimited
};

// ---- Profession definition ----
struct ProfessionDefinition {
    std::string name;
    std::string description;
    std::string abilityDescription;

    /// Minimum stat values required (before race mods are applied)
    /// Key = stat name ("STY","FYS","SMI","INT","PSY","KAR")
    std::map<std::string, int> statRequirements;

    int maxSkillChoices;     ///< How many profession skills player may choose (9 or 12)

    int maxWeaponSkills;     ///< Max weapon-skill picks from profession list (-1 = unlimited from list)
    int maxLanguages;        ///< Max spoken foreign languages
    int maxLiteracy;         ///< Max read/write foreign languages
    int maxInstruments;      ///< Max instrument picks

    /// List of skills this profession may pick from.
    std::vector<SkillId> possibleSkills;

    bool isWarriorExpansion = false;  ///< True if this profession is from the Warrior Expansion
};

// ---- BP level (Warrior Expansion: Vanlig / Extraordinär / Hjälte) ----
struct BPLevelDefinition {
    std::string name;             ///< e.g. "Vanlig", "Extraordinär", "Hjälte"
    int bp;                       ///< Total BP available
    int specialAbilityRolls;      ///< Number of special ability rolls
    int ep[4];                    ///< EP by age: [0]=Young, [1]=Mature, [2]=Middle, [3]=Old
    int maxFV[4];                 ///< Max start FV by age: [0]=Young, [1]=Mature, [2]=Middle, [3]=Old
};

// ---- Age category ----
struct AgeCategory {
    std::string name;
    int statMods[STAT_COUNT];
    int ep;
    double capitalMultiplier;
    int maxStartFV;
    /// Age ranges per race (informational only)
    std::string ageRangeNote;
};

// ---- Social standing entry ----
struct SocialStandingEntry {
    int  rollMin;
    int  rollMax;
    std::string description;
    int  baseCapital;  ///< silver marks (sm)
};

// ---- GameData: singleton-like accessor for all static tables ----
namespace GameData {

const std::vector<RaceDefinition>&      getRaces();
const std::vector<ProfessionDefinition>& getProfessions();
const std::vector<SkillDefinition>&     getAllSkills();
const std::vector<SkillDefinition>&     getPrimarySkills();
const std::vector<SkillDefinition>&     getSecondarySkills();
const std::vector<AgeCategory>&         getAgeCategories();

/// Returns structured skill grants for a Special Ability roll total.
/// Fixed-skill grants have playerChoice=false; "any skill" grants have playerChoice=true.
std::vector<SpecialAbilityGrant> getSpecialAbilityGrants(int rollTotal);

/// Warrior Expansion: BP level table (Vanlig / Extraordinär / Hjälte).
const std::vector<BPLevelDefinition>& getBPLevels();

/// Warrior Expansion: structured skill grants for a Warrior Special Ability roll.
std::vector<SpecialAbilityGrant> getWarriorSpecialAbilityGrants(int rollTotal);

/// Warrior Expansion: description string for a Warrior Special Ability roll.
std::string lookupWarriorSpecialAbility(int rollTotal);

const std::vector<SocialStandingEntry>& getSocialStandingTable();

/// Returns the profession definition with the given name, or nullopt.
std::optional<ProfessionDefinition>     findProfession(const std::string& name);

/// Returns the race definition with the given name, or nullopt.
std::optional<RaceDefinition>           findRace(const std::string& name);

/// Returns skill definition by name, or nullopt.
std::optional<SkillDefinition>          findSkill(const std::string& name);

/// Returns skill definition by id, or nullopt.
std::optional<SkillDefinition>          findSkill(SkillId id);

} // namespace GameData
} // namespace dod
