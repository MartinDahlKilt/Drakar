#pragma once
#include "core/SkillId.h"
#include <string>
#include <vector>
#include <optional>
#include <array>

namespace dod {

/// The seven base statistics (indices match StatIndex enum in GameData.h).
struct Stats {
    int STY = 0;  ///< Styrka  (Strength)
    int FYS = 0;  ///< Fysik   (Endurance/Health)
    int SMI = 0;  ///< Smidighet (Agility)
    int INT = 0;  ///< Intelligens
    int PSY = 0;  ///< Psyke   (Psyche/willpower)
    int KAR = 0;  ///< Karisma (Charisma)
    int STO = 0;  ///< Storlek (Size)
};

/// HP per body part (indices: 0=BröstKorg, 1=Huvud, 2=VänsterArm,
///                            3=HögerArm, 4=VänsterBen, 5=HögerBen)
struct BodyPartHP {
    int brostKorg   = 0;  ///< Chest
    int huvud       = 0;  ///< Head
    int vansterArm  = 0;  ///< Left arm
    int hogerArm    = 0;  ///< Right arm
    int vansterBen  = 0;  ///< Left leg
    int hogerBen    = 0;  ///< Right leg
};

/// Derived statistics (calculated from base stats).
struct DerivedStats {
    int         hpTotal      = 0;  ///< Total hit points
    BodyPartHP  hpParts;
    std::string damageBonus;       ///< e.g. "+1T6", "none"
    int         movement     = 0;  ///< Movement in squares per combat round
};

/// A single skill entry on the character sheet.
struct SkillEntry {
    std::string name;
    std::string baseStat;    ///< e.g. "INT"
    SkillId     skillId = SkillId::PLAYER_CHOICE;  ///< Base skill type (PLAYER_CHOICE for custom entries)
    int         fv     = 0;  ///< Färdighetsvärde (skill value)
    int         fvBase = 0;  ///< Original FV (BC or SA-granted); EP refunds stop here
    std::string clNote;      ///< Display-only CL bonus note, e.g. "+3 CL (Special Ability)"
    bool        isPrimary = false;
    bool        isProfessionSkill = false;
};

/// A skill bonus granted by a Special Ability roll.
struct SpecialAbilityGrant {
    enum class Type { FV, CL };
    Type        type;
    SkillId     skillId = SkillId::PLAYER_CHOICE;  ///< PLAYER_CHOICE = player must choose
    int         amount = 0;
    bool        playerChoice = false;
};

/// Records how a dice roll was made (for display to the user).
struct RollRecord {
    std::string context;   ///< e.g. "Weapon hand"
    std::string diceExpr;  ///< e.g. "2T6+3"
    std::vector<int> rolls;
    int modifier = 0;
    int total    = 0;
    std::string outcome;   ///< Human-readable result
};

/// The complete character data model.
struct Character {
    // --- Identity ---
    std::string name;
    std::string gender;         ///< "Male" / "Female"
    std::string race;
    std::string profession;
    std::string ageCategory;    ///< e.g. "Mature (Mogen)"

    // --- Stats at each stage ---
    Stats purchasedStats;       ///< Values as bought with BP (before any mods)
    Stats raceMods;             ///< Modifiers from chosen race
    Stats ageMods;              ///< Modifiers from chosen age category
    Stats finalStats;           ///< purchasedStats + raceMods + ageMods

    /// Derived stats (calculated from finalStats)
    DerivedStats derived;

    // --- Background Points ---
    int bpTotal     = 125;
    int bpSpent     = 0;
    int bpRemaining = 125;

    // --- STO ---
    int stoNormal   = 0;   ///< Race's normal STO
    int stoAdjustment = 0; ///< +/- from normal (costs 2 BP each)

    // --- Dice roll records (shown to user, stored in JSON for transparency) ---
    std::vector<RollRecord> rollHistory;

    // --- Special ability ---
    bool                          hasSpecialAbility = false;
    int                           specialAbilityBPSpent = 0;
    std::string                   specialAbilityDesc;

    // --- Social / economic ---
    int         socialRoll        = 0;
    std::string socialDescription;
    int         capitalBase        = 0;  ///< Base capital from dice roll (before age multiplier)
    int         startingCapital   = 0;  ///< In silver marks (sm), after age multiplier

    // --- Weapon hand ---
    int         weaponHandRoll    = 0;
    std::string weaponHand;             ///< "Right", "Left", "Double handed", "Ambidextrous"
    bool        weaponHandFromSpecialAbility = false;  ///< true if granted automatically by Special Ability

    // --- Skills ---
    std::vector<SkillEntry> skills;     ///< All skills (primary + chosen profession + SA-granted)

    // --- Special ability grants (structured, for reference) ---
    std::vector<SpecialAbilityGrant> specialAbilityGrants;

    // --- Experience Points ---
    int epFromAge   = 0;
    int epFromBP    = 0;   ///< remainingBP * 5 (after all purchases)
    int epTotal     = 0;
    int epSpent     = 0;   ///< EP consumed raising skill FVs

    // --- Background text ---
    std::string appearance;
    std::string personality;
    std::string background;
};

} // namespace dod
