#include "core/GameRules.h"
#include "core/GameData.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

namespace dod {
namespace GameRules {

// ---------------------------------------------------------------------------
// BP cost table — stat values 3 through 18
// Index 0 = value 3, index 15 = value 18
// ---------------------------------------------------------------------------
static constexpr int BP_COST_TABLE[] = {
//  3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18
    0,  1,  2,  3,  5,  7,  9, 10, 11, 12, 14, 17, 20, 25, 30, 40
};
static constexpr int BP_TABLE_MIN = 3;
static constexpr int BP_TABLE_MAX = 18;

int getStatBPCost(int value) {
    if (value < BP_TABLE_MIN || value > BP_TABLE_MAX) {
        throw std::out_of_range(
            "Stat value " + std::to_string(value) +
            " is outside the valid range [3, 18]");
    }
    return BP_COST_TABLE[value - BP_TABLE_MIN];
}

int getStatRaiseCost(int fromValue, int toValue) {
    return getStatBPCost(toValue) - getStatBPCost(fromValue);
}

// ---------------------------------------------------------------------------
// Base chance (BC)
// ---------------------------------------------------------------------------
int calculateBC(int statValue) {
    if (statValue <= 3)  return 0;
    if (statValue <= 8)  return 1;
    if (statValue <= 12) return 2;
    if (statValue <= 16) return 3;
    if (statValue <= 20) return 4;
    return 5;  // >20
}

// ---------------------------------------------------------------------------
// Total hit points
// ---------------------------------------------------------------------------
int calculateHP(int fys, int sto) {
    return static_cast<int>(std::ceil((fys + sto) / 2.0));
}

// ---------------------------------------------------------------------------
// Body-part HP table
// Source: rules-book table (Röst=korg/hals/mag, Huvud, armar, ben)
// The table in the rules gives values per total-HP level.
// Approximation used: round for partial values.
// ---------------------------------------------------------------------------
BodyPartHP calculateBodyPartHP(int totalHP) {
    // Approximation from the rules table:
    // BröstKorg = ceil(totalHP / 2)
    // Huvud     = ceil(totalHP / 3)
    // Each arm  = ceil(totalHP / 4)
    // Each leg  = ceil(totalHP / 3)
    BodyPartHP hp;
    hp.brostKorg  = static_cast<int>(std::ceil(totalHP / 2.0));
    hp.huvud      = static_cast<int>(std::ceil(totalHP / 3.0));
    hp.vansterArm = static_cast<int>(std::ceil(totalHP / 4.0));
    hp.hogerArm   = hp.vansterArm;
    hp.vansterBen = static_cast<int>(std::ceil(totalHP / 3.0));
    hp.hogerBen   = hp.vansterBen;
    return hp;
}

// ---------------------------------------------------------------------------
// Damage bonus
// ---------------------------------------------------------------------------
std::string calculateDamageBonus(int sty, int sto) {
    int sum = sty + sto;
    if (sum <= 26)  return "none";
    if (sum <= 29)  return "+1T2";
    if (sum <= 32)  return "+1T4";
    if (sum <= 40)  return "+1T6";
    if (sum <= 50)  return "+2T6";
    if (sum <= 60)  return "+3T6";
    return "+4T6";            // 61-80 and beyond
}

// ---------------------------------------------------------------------------
// Movement table
// Source: rules table based on STO+FYS+SMI sum
// Anka -2 squares; Alv +2 in some editions — reflecting common rulings
// ---------------------------------------------------------------------------
int calculateMovement(int sto, int fys, int smi, const std::string& raceName) {
    int sum = sto + fys + smi;

    // Base lookup table (sum → movement squares), per rules table
    int base;
    if      (sum <= 11) base =  7;
    else if (sum <= 20) base =  8;
    else if (sum <= 29) base =  9;
    else if (sum <= 38) base = 10;
    else if (sum <= 47) base = 11;
    else if (sum <= 56) base = 12;
    else if (sum <= 65) base = 13;
    else if (sum <= 74) base = 14;
    else if (sum <= 83) base = 15;
    else if (sum <= 92) base = 16;
    else                base = 16 + (sum - 92) / 8;

    // Race modifier
    if (raceName == "Anka") base -= 2;

    return std::max(1, base);
}

// ---------------------------------------------------------------------------
// Social standing
// ---------------------------------------------------------------------------
std::string getSocialStandingDesc(int roll) {
    for (const auto& entry : GameData::getSocialStandingTable()) {
        if (roll >= entry.rollMin && roll <= entry.rollMax)
            return entry.description;
    }
    // Default to highest tier if over max
    return GameData::getSocialStandingTable().back().description;
}

int getStartingCapitalBase(int roll) {
    for (const auto& entry : GameData::getSocialStandingTable()) {
        if (roll >= entry.rollMin && roll <= entry.rollMax)
            return entry.baseCapital;
    }
    return GameData::getSocialStandingTable().back().baseCapital;
}

int calculateStartingCapital(int baseCapital, double ageMultiplier) {
    double raw = baseCapital * ageMultiplier;
    // Round to nearest 50
    return static_cast<int>(std::round(raw / 50.0) * 50);
}

// ---------------------------------------------------------------------------
// Profession requirements
// ---------------------------------------------------------------------------
bool meetsStatRequirements(const Stats& stats, const ProfessionDefinition& prof) {
    for (const auto& [statName, minVal] : prof.statRequirements) {
        int actual = 0;
        if      (statName == "STY") actual = stats.STY;
        else if (statName == "FYS") actual = stats.FYS;
        else if (statName == "SMI") actual = stats.SMI;
        else if (statName == "INT") actual = stats.INT;
        else if (statName == "PSY") actual = stats.PSY;
        else if (statName == "KAR") actual = stats.KAR;
        else if (statName == "STO") actual = stats.STO;

        if (actual < minVal) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// STO adjustment cost  (2 BP per ±1 from normal, linear)
// ---------------------------------------------------------------------------
int getSTOAdjustmentCost(int delta) {
    return std::abs(delta) * 2;
}

int getSTOReductionRefund(int delta) {
    return std::abs(delta) * 2;
}

// ---------------------------------------------------------------------------
// EP skill advancement
// ---------------------------------------------------------------------------
int getEPCostMultiplier(int targetFV) {
    if (targetFV <= 10) return 1;
    if (targetFV <= 14) return 2;
    if (targetFV <= 17) return 3;
    if (targetFV <= 20) return 4;
    return 5;
}

int getEPStepCost(int toFV, bool isPrimary, bool isProfession) {
    int base = isPrimary ? 2 : (isProfession ? 3 : 5);
    return base * getEPCostMultiplier(toFV);
}

int calculateEPCost(int fromFV, int toFV, bool isPrimary, bool isProfession) {
    if (toFV <= fromFV) return 0;
    int total = 0;
    for (int fv = fromFV + 1; fv <= toFV; ++fv)
        total += getEPStepCost(fv, isPrimary, isProfession);
    return total;
}

int getAgeFVCap(const std::string& ageCategory) {
    if (ageCategory.find("Young")  != std::string::npos) return 13;
    if (ageCategory.find("Mature") != std::string::npos) return 15;
    if (ageCategory.find("Mid")    != std::string::npos) return 17;
    if (ageCategory.find("Old")    != std::string::npos) return 19;
    return 20;
}

} // namespace GameRules
} // namespace dod
