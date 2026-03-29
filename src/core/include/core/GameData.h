#pragma once
#include "core/Character.h"
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
    std::string skillName;
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
    std::string name;
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

    /// List of skill names this profession may pick from.
    /// Weapon slots are represented as "WEAPON_SLOT" entries (expanded at runtime).
    std::vector<std::string> possibleSkills;
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
const std::vector<SocialStandingEntry>& getSocialStandingTable();

/// Returns the profession definition with the given name, or nullopt.
std::optional<ProfessionDefinition>     findProfession(const std::string& name);

/// Returns the race definition with the given name, or nullopt.
std::optional<RaceDefinition>           findRace(const std::string& name);

/// Returns skill definition by name, or nullopt.
std::optional<SkillDefinition>          findSkill(const std::string& name);

} // namespace GameData
} // namespace dod
