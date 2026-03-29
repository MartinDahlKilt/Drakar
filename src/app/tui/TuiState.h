#pragma once
#include "core/Character.h"
#include "core/GameData.h"
#include "core/GameRules.h"
#include <set>
#include <string>

namespace dod {

// ---------------------------------------------------------------------------
// BP breakdown — tracks how many BP were spent in each category so
// individual categories can be refunded independently.
// ---------------------------------------------------------------------------
struct BPBreakdown {
    int forRace           = 0;
    int forStats          = 0;  ///< sum of getStatBPCost() for all 6 purchased stats
    int forSTO            = 0;  ///< cost of STO adjustment only
    int forSpecialAbility = 0;
    int forWeaponHand     = 0;
    int forSocial         = 0;
    int forCapital        = 0;
    int forSyn            = 0;  ///< BP spent on sight roll (Warrior Expansion)
    int forHorsel         = 0;  ///< BP spent on hearing roll (Warrior Expansion)

    int total() const {
        return forRace + forStats + forSTO + forSpecialAbility
             + forWeaponHand + forSocial + forCapital
             + forSyn + forHorsel;
    }
};

// ---------------------------------------------------------------------------
// Sections — every navigable screen in the TUI.
// ---------------------------------------------------------------------------
enum class Section {
    Dashboard,
    Name,
    BPLevel,         ///< Warrior Expansion: choose BP tier (Vanlig/Extraordinär/Hjälte)
    Profession,
    Race,
    Stats,
    STO,
    SynHorsel,       ///< Warrior Expansion: sight and hearing rolls
    SpecialAbility,
    WeaponHand,
    Social,
    Capital,
    Age,
    Skills,
    Appearance,
    Summary,
    Settings,
    COUNT
};

// ---------------------------------------------------------------------------
// Central state passed by reference to every screen.
// ---------------------------------------------------------------------------
struct TuiState {
    Character   character;           ///< The character being built
    BPBreakdown bpBreakdown;         ///< Per-category BP spend

    Section     currentSection = Section::Dashboard;
    Section     lastSection    = Section::Profession; ///< last non-Dashboard section visited
    Section     pendingNavigate = Section::Dashboard;
    bool        navigateRequest = false;

    // ---- Settings ----
    bool            allowAnka              = false;  ///< Anka race is hidden by default
    bool            allowWarriorExpansion  = false;  ///< Warrior Expansion enabled

    // ---- Warrior Expansion state ----
    int  bpLevelIndex  = 0;  ///< 0=Vanlig(125), 1=Extraordinär(150), 2=Hjälte(175)

    std::set<Section> completed;    ///< Sections marked done

    // ---- Transient state needed across related screens ----
    int  socialBPSpent  = 0;        ///< Passed from Social screen to Capital screen

    // ---- Computed helpers ----
    int bpTotal() const {
        if (allowWarriorExpansion) {
            const auto& levels = GameData::getBPLevels();
            if (bpLevelIndex >= 0 && bpLevelIndex < (int)levels.size())
                return levels[bpLevelIndex].bp;
        }
        return 125;
    }
    int bpRemaining() const { return bpTotal() - bpBreakdown.total(); }

    /// Live EP pool: age EP + remaining BP × 5.
    int computeEPTotal()   const { return character.epFromAge + bpRemaining() * 5; }
    /// EP still available to spend.
    int epAvailable()      const { return computeEPTotal() - character.epSpent; }

    bool isComplete(Section s) const { return completed.count(s) > 0; }

    void markComplete(Section s) { completed.insert(s); }

    // Returns true if all mandatory sections are done (name, profession, race,
    // stats, age, skills — minimum for a valid character) AND no final stat
    // is zero or below.
    bool isReadyToSave() const {
        for (Section s : {Section::Name, Section::Profession, Section::Race,
                          Section::Stats, Section::Age, Section::Skills}) {
            if (!isComplete(s)) return false;
        }
        if (allowWarriorExpansion) {
            if (!isComplete(Section::BPLevel))    return false;
            if (!isComplete(Section::SynHorsel))  return false;
        }
        const auto f = computeFinalStats();
        if (f.STY <= 0 || f.FYS <= 0 || f.SMI <= 0 || f.INT <= 0 ||
            f.PSY <= 0 || f.KAR <= 0 || f.STO <= 0) return false;
        return true;
    }

    /// Returns true if the current profession is a Warrior Expansion profession.
    bool isWarriorProfession() const {
        const auto& name = character.profession;
        if (name.empty()) return false;
        auto opt = GameData::findProfession(name);
        return opt.has_value() && opt->isWarriorExpansion;
    }

    // ---- Stat helpers ----

    /// purchasedStats + raceMods + ageMods
    Stats computeFinalStats() const {
        const auto& p = character.purchasedStats;
        const auto& r = character.raceMods;
        const auto& a = character.ageMods;
        Stats f;
        f.STY = p.STY + r.STY + a.STY;
        f.FYS = p.FYS + r.FYS + a.FYS;
        f.SMI = p.SMI + r.SMI + a.SMI;
        f.INT = p.INT + r.INT + a.INT;
        f.PSY = p.PSY + r.PSY + a.PSY;
        f.KAR = p.KAR + r.KAR + a.KAR;
        f.STO = p.STO + r.STO + a.STO;
        return f;
    }

    /// Full BP refund for stats + STO; reset purchasedStats to 3.
    void refundStats() {
        bpBreakdown.forStats = 0;
        bpBreakdown.forSTO   = 0;
        character.purchasedStats = {3, 3, 3, 3, 3, 3, 0};  // STO handled by race
        character.stoAdjustment  = 0;
        // STO will be reset to race normal when race is re-set
        completed.erase(Section::Stats);
        completed.erase(Section::STO);
    }

    /// Refund the race BP cost and call refundStats().
    void refundRace() {
        bpBreakdown.forRace = 0;
        character.raceMods = {};
        character.stoNormal = 0;
        character.purchasedStats.STO = 0;
        refundStats();
        completed.erase(Section::Race);
    }
};

} // namespace dod
