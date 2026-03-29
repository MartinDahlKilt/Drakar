#pragma once
#include "core/Character.h"
#include "core/GameData.h"
#include <string>
#include <map>

namespace dod {
namespace GameRules {

// ---------------------------------------------------------------------------
// Background Points (BP) costs
// ---------------------------------------------------------------------------

/// Returns the BP cost to buy a stat at the given value (3-18).
/// @throws std::out_of_range if value < 3 or > 18.
int getStatBPCost(int value);

/// Returns the marginal BP cost to raise a stat from `fromValue` to `toValue`.
/// Equivalent to getStatBPCost(toValue) - getStatBPCost(fromValue).
int getStatRaiseCost(int fromValue, int toValue);

// ---------------------------------------------------------------------------
// Skill base chance (BC)
// ---------------------------------------------------------------------------

/// Returns the base FV granted free for a skill based on the relevant stat value.
/// Formula: stat 1-3→0, 4-8→1, 9-12→2, 13-16→3, 17-20→4, >20→5
int calculateBC(int statValue);

// ---------------------------------------------------------------------------
// Derived statistics
// ---------------------------------------------------------------------------

/// Total hit points = ceil((FYS + STO) / 2)
int calculateHP(int fys, int sto);

/// HP distribution per body part based on total HP.
BodyPartHP calculateBodyPartHP(int totalHP);

/// Damage bonus string based on STY + STO sum.
/// Returns e.g. "none", "+1T2", "+1T4", "+1T6", "+2T6", "+3T6", "+4T6"
std::string calculateDamageBonus(int sty, int sto);

/// Movement in squares per combat round, based on STO + FYS + SMI sum and race name.
int calculateMovement(int sto, int fys, int smi, const std::string& raceName);

// ---------------------------------------------------------------------------
// Social standing & starting capital
// ---------------------------------------------------------------------------

/// Returns the social standing description for a roll (2T6 + BP).
std::string getSocialStandingDesc(int roll);

/// Returns the base starting capital (in silver marks) for a social standing roll.
int getStartingCapitalBase(int roll);

/// Returns the actual starting capital after age multiplier is applied.
/// capitalBase * ageMultiplier, rounded to nearest 50 sm.
int calculateStartingCapital(int baseCapital, double ageMultiplier);

// ---------------------------------------------------------------------------
// Profession requirements
// ---------------------------------------------------------------------------

/// Returns true if `stats` meet the minimum stat requirements for `prof`.
/// Checks are performed against the supplied stats directly (typically
/// purchasedStats + raceMods, before age mods).
bool meetsStatRequirements(const Stats& stats,
                            const ProfessionDefinition& prof);

// ---------------------------------------------------------------------------
// STO helpers
// ---------------------------------------------------------------------------

/// Returns the BP cost to adjust STO by `delta` (+/-) from the race normal.
/// Delta of +1 or -1 = 2 BP; larger = linear.
int getSTOAdjustmentCost(int delta);

/// Returns the BP refund for reducing STO below the race normal (same as cost).
int getSTOReductionRefund(int delta);

// ---------------------------------------------------------------------------
// EP (Experience Point) skill advancement
// ---------------------------------------------------------------------------

/// Returns the multiplier for spending EP to raise to targetFV.
/// Bands: 1-10->1, 11-14->2, 15-17->3, 18-20->4, 21+->5.
int getEPCostMultiplier(int targetFV);

/// Returns the EP cost for one FV step (from->from+1) for the given skill category.
/// baseCost: primary=2, profession=3, secondary=5.
int getEPStepCost(int toFV, bool isPrimary, bool isProfession);

/// Total EP cost to raise a skill from fromFV to toFV (summed per step).
int calculateEPCost(int fromFV, int toFV, bool isPrimary, bool isProfession);

/// Maximum FV purchaseable at character creation for the given age category.
/// Young->13, Mature->15, Mid-aged->17, Old->19, other->20.
int getAgeFVCap(const std::string& ageCategory);

} // namespace GameRules
} // namespace dod
