#include "DiceScreen.h"
#include "../TuiTheme.h"

#include "core/GameRules.h"
#include "core/Dice.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <algorithm>
#include <cmath>
#include <sstream>

using namespace ftxui;

namespace dod {

// ---------------------------------------------------------------------------
// Special ability lookup (ported from old CharacterCreator.cpp)
// ---------------------------------------------------------------------------
static std::string lookupSpecialAbility(int roll) {
    if (roll <= 4)  return "Bonus FV+1 in any one secondary skill.";
    if (roll <= 6)  return "Seafaring background: +2 FV in Sjökunnighet and Navigera.";
    if (roll <= 8)  return "Strong ankles: +3 FV to Hoppa.";
    if (roll <= 10) return "Street brawler upbringing: +3 FV to Slagsmål.";
    if (roll <= 12) return "Craftsman background: +3 FV in one chosen Hantverk.";
    if (roll <= 14) return "Nimble frame: +3 FV to Akrobatik.";
    if (roll <= 16) return "Merchant background: +3 FV to Värdera.";
    if (roll <= 18) return "Excellent co-ordination: +3 FV to Två vapen.";
    if (roll <= 20) return "Hobbyist: FV 3 in one chosen secondary skill.";
    if (roll <= 22) return "Strong grip: +3 to CL in Klättra.";
    if (roll <= 24) return "Receptive medium: +5 CL in Magisk kanalisering.";
    if (roll <= 26) return "Devoted student: +2 FV on any one skill (raises cap by 2).";
    if (roll <= 28) return "Convincing tone: +3 CL in Övertala and Muta.";
    if (roll <= 30) return "Sixth sense: +1 FV in Upptäcka fara and Finna dolda ting.";
    if (roll <= 32) return "Piercing gaze: +5 CL in Hypnotisera.";
    if (roll <= 34) return "Magic sensitivity: +5 CL in Känna magi.";
    if (roll <= 36) return "Linguistic talent: FV 20 in one spoken and written language.";
    if (roll <= 38) return "Wide expertise: choose 2 extra secondary skills as profession skills.";
    if (roll <= 40) return "Skilled archer: all projectile weapon ranges +25%.";
    if (roll <= 42) return "Perfect pitch: instrument/singing base cost always 1.";
    if (roll <= 44) return "Precision sense: CL +1 on all weapon skills.";
    if (roll <= 46) return "Ambidextrous: use both hands without penalty.";
    if (roll <= 48) return "Perfect time sense: always know time within 10 minutes.";
    if (roll <= 51) return "Absolute eye: judge distances within 5% margin.";
    if (roll <= 54) return "Very observant: +2 CL in Finna dolda ting and Upptäcka fara.";
    if (roll == 55) return "Lightning reflexes: +3 on all initiative rolls.";
    if (roll == 56) return "Berserker: +5 FV in Bärsärkagång.";
    if (roll == 57) return "Perfect balance: +5 SMI on all balance acts.";
    if (roll == 58) return "Master horseman: +10 FV in Rida; never thrown by a horse.";
    if (roll == 59) return "Ambidextrous (superior): full bilateral use of both hands.";
    if (roll == 60) return "Animal friend: never attacked by normal animals.";
    if (roll == 61) return "Lucky: spend 1 PSY for +1 CL on any skill.";
    if (roll == 62) return "Magical empathy: identify spells on objects using PSY.";
    if (roll == 63) return "Deity's favoured: 25% chance gods restore all HP at 0.";
    if (roll == 64) return "Quick learner: secondary skill base cost reduced to 4.";
    if (roll == 65) return "Pain tolerance: total HP ×1.5.";
    if (roll == 66) return "Lightning striker: always acts first in combat.";
    if (roll == 67) return "Foe-bane: +5 CL against one sworn enemy race/group.";
    if (roll == 68) return "Body control: raise STY-skills by +5 for 3 rounds (twice/day).";
    if (roll == 69) return "Iron fist: always deals maximum unarmed damage.";
    if (roll == 70) return "Fearless: -5 on all Fear Table rolls.";
    if (roll == 71) return "Indomitable will: +5 PSY on PSY vs PSY resistance rolls.";
    if (roll == 72) return "Elemental hardy: +5 FYS vs elemental resistance rolls.";
    if (roll == 73) return "Fast healing: physical damage heals at double normal rate.";
    if (roll == 74) return "Mental recovery: PSY-point recovery time halved.";
    if (roll == 75) return "Natural weapon talent: +5 FV in one chosen weapon skill.";
    if (roll == 76) return "Split personality: gain ability of one other chosen profession.";
    if (roll == 77) return "Professional mastery: cost to learn one spell/skill halved.";
    if (roll == 78) return "Shapeshifter: may transform into an animal (T6: 1=wolf,2=bear,3=hawk,4=deer,5=swan,6=cat).";
    if (roll == 79) return "Quick perception: +5 CL to parry; can parry projectiles you can see.";
    if (roll == 80) return "Psy potential: -5 on all PSY advancement rolls.";
    return "+1 on three different chosen stats, or +2 on one chosen stat.";
}


// ---------------------------------------------------------------------------
// Context metadata
// ---------------------------------------------------------------------------
struct CtxMeta {
    std::string title;
    std::string description;
    int  diceCount;
    int  diceSides;
    bool bpIsBonus;  // true = BP added as modifier to roll
    int  maxBP;
};

static CtxMeta getCtxMeta(DiceContext ctx, const TuiState& state) {
    switch (ctx) {
        case DiceContext::SpecialAbility:
            return {
                "Special Ability",
                "Roll 2T20 + BP spent. The higher the roll, the better the ability.\n"
                "Maximum 40 BP bonus. Spending BP improves your chances.\n"
                "Result table: 2-4 minor bonus … 80 best ability.",
                2, 20, true, 40
            };
        case DiceContext::WeaponHand:
            return {
                "Weapon Hand",
                "Roll 2T6 + optional BP bonus.\n"
                "2-11  = Right hand\n"
                "12-14 = Left hand\n"
                "15-18 = Double handed\n"
                "19+   = Ambidextrous",
                2, 6, true, std::min(state.bpRemaining(), 12)
            };
        case DiceContext::Social:
            return {
                "Social Standing",
                "Roll 2T6 + optional BP bonus to determine your social background.\n"
                "Higher roll = higher social status and more starting capital.\n"
                "BP spent here also partially boosts your capital roll.",
                2, 6, true, state.bpRemaining()
            };
        case DiceContext::Capital: {
            int socialBonus = (int)std::ceil(state.socialBPSpent / 2.0);
            return {
                "Starting Capital",
                "Roll 2T6 + optional BP + half of social BP spent (+" + std::to_string(socialBonus) + " from " + std::to_string(state.socialBPSpent) + " social BP).\n"
                "Result determines base capital before age multiplier is applied.\n"
                "Age multiplier is applied after you choose an age category.",
                2, 6, true, state.bpRemaining()
            };
        }
    }
    return {"?","",2,6,false,0};
}

static std::string interpretResult(DiceContext ctx, int total, const TuiState& /*state*/) {
    switch (ctx) {
        case DiceContext::SpecialAbility:
            return lookupSpecialAbility(total);
        case DiceContext::WeaponHand:
            if (total >= 19) return "Ambidextrous";
            if (total >= 15) return "Double handed";
            if (total >= 12) return "Left hand";
            return "Right hand";
        case DiceContext::Social:
            return GameRules::getSocialStandingDesc(total);
        case DiceContext::Capital: {
            int base = GameRules::getStartingCapitalBase(total);
            return std::to_string(base) + " sm (base, before age multiplier)";
        }
    }
    return "";
}

// ---------------------------------------------------------------------------
// Factory
// ---------------------------------------------------------------------------
ftxui::Component MakeDiceScreen(TuiApp& app, DiceContext ctx) {
    using namespace theme;

    auto meta = std::make_shared<CtxMeta>(getCtxMeta(ctx, app.state()));

    // BP bonus — adjusted with ← / →
    auto bpBonus = std::make_shared<int>(0);

    // State
    auto hasRolled = std::make_shared<bool>(false);
    auto rollTotal = std::make_shared<int>(0);
    auto rollDice  = std::make_shared<std::vector<int>>();
    auto resultStr = std::make_shared<std::string>("");
    auto statusMsg = std::make_shared<std::string>("");

    // Pre-load locked result if already rolled
    {
        auto& state = app.state();
        if (ctx == DiceContext::SpecialAbility && state.character.hasSpecialAbility) {
        *hasRolled  = true;
        // We don't store individual dice — just show description
        *resultStr  = state.character.specialAbilityDesc;
    } else if (ctx == DiceContext::WeaponHand && !state.character.weaponHand.empty()) {
        *hasRolled  = true;
        *rollTotal  = state.character.weaponHandRoll;
        *resultStr  = state.character.weaponHandFromSpecialAbility
                      ? state.character.weaponHand + " (granted by Special Ability)"
                      : state.character.weaponHand;
    } else if (ctx == DiceContext::Social && !state.character.socialDescription.empty()) {
        *hasRolled  = true;
        *rollTotal  = state.character.socialRoll;
        *resultStr  = state.character.socialDescription;
    } else if (ctx == DiceContext::Capital && state.isComplete(Section::Capital)) {
        *hasRolled  = true;
        *rollTotal  = 0;
        *resultStr  = std::to_string(state.character.startingCapital) + " sm (locked)";
        }
    }

    // Layout — no interactive input widget needed
    auto layout = Container::Vertical({});

    auto doRoll = [&app, ctx, meta, bpBonus, hasRolled, rollTotal, rollDice, resultStr, statusMsg]() {
        auto& state = app.state();
        if (*hasRolled) return;

        int bp = *bpBonus;
        if (bp < 0) bp = 0;
        if (bp > meta->maxBP) bp = meta->maxBP;
        if (bp > state.bpRemaining()) {
            *statusMsg = "Not enough BP! You have " + std::to_string(state.bpRemaining()) + " remaining.";
            return;
        }

        // For capital, add half of social BP
        int modifier = bp;
        if (ctx == DiceContext::Capital) {
            modifier += (int)std::ceil(state.socialBPSpent / 2.0);
        }

        auto result = app.dice().roll(meta->diceCount, meta->diceSides, modifier);
        *rollDice  = result.rolls;
        *rollTotal = result.total;
        *resultStr = interpretResult(ctx, result.total, state);
        *hasRolled = true;
        *statusMsg = "";

        // Record roll
        RollRecord rec;
        rec.context   = meta->title;
        rec.diceExpr  = std::to_string(meta->diceCount) + "T"
                      + std::to_string(meta->diceSides)
                      + (modifier >= 0 ? "+" : "") + std::to_string(modifier);
        rec.rolls     = result.rolls;
        rec.modifier  = modifier;
        rec.total     = result.total;
        rec.outcome   = *resultStr;
        state.character.rollHistory.push_back(rec);

        // Apply to state
        switch (ctx) {
            case DiceContext::SpecialAbility:
                state.character.hasSpecialAbility     = true;
                state.character.specialAbilityBPSpent = bp;
                state.character.specialAbilityDesc    = *resultStr;
                state.bpBreakdown.forSpecialAbility   = bp;
                state.markComplete(Section::SpecialAbility);
                // Apply structured skill grants
                {
                    auto grants = GameData::getSpecialAbilityGrants(result.total);
                    state.character.specialAbilityGrants = grants;
                    auto finalStats = state.computeFinalStats();
                    for (const auto& g : grants) {
                        if (g.playerChoice) continue;  // handled separately
                        std::string sname = g.skillName;
                        // Find existing entry
                        auto it = std::find_if(state.character.skills.begin(),
                                               state.character.skills.end(),
                                               [&](const SkillEntry& e){ return e.name == sname; });
                        if (g.type == SpecialAbilityGrant::Type::FV) {
                            if (it != state.character.skills.end()) {
                                // Apply to existing primary or profession entry
                                if (it->fvBase == 0) it->fvBase = it->fv;
                                it->fv += g.amount;
                            } else {
                                // Determine correct category for the new entry
                                bool isPrim = false;
                                for (const auto& ps : GameData::getPrimarySkills())
                                    if (ps.name == sname) { isPrim = true; break; }
                                SkillEntry e;
                                e.name    = sname;
                                auto def  = GameData::findSkill(sname);
                                e.baseStat = def ? def->baseStat : "var.";
                                int bc    = GameRules::calculateBC(def ? [&](){
                                    const auto& fs = finalStats;
                                    const auto& bs = e.baseStat;
                                    if (bs=="STY") return fs.STY;
                                    if (bs=="FYS") return fs.FYS;
                                    if (bs=="SMI") return fs.SMI;
                                    if (bs=="INT") return fs.INT;
                                    if (bs=="PSY") return fs.PSY;
                                    if (bs=="KAR") return fs.KAR;
                                    if (bs=="STO") return fs.STO;
                                    return 10;
                                }() : 10);
                                e.fv      = bc + g.amount;
                                e.fvBase  = bc + g.amount;
                                e.isPrimary         = isPrim;
                                e.isProfessionSkill = false;
                                state.character.skills.push_back(e);
                            }
                        } else { // CL grant — note only
                            std::string note = "+" + std::to_string(g.amount) + " CL (Special Ability)";
                            if (it != state.character.skills.end()) {
                                if (!it->clNote.empty()) it->clNote += ", ";
                                it->clNote += note;
                            } else {
                                bool isPrim = false;
                                for (const auto& ps : GameData::getPrimarySkills())
                                    if (ps.name == sname) { isPrim = true; break; }
                                SkillEntry e;
                                e.name    = sname;
                                auto def  = GameData::findSkill(sname);
                                e.baseStat = def ? def->baseStat : "var.";
                                int bc    = GameRules::calculateBC(def ? [&](){
                                    const auto& fs = finalStats;
                                    const auto& bs = e.baseStat;
                                    if (bs=="STY") return fs.STY;
                                    if (bs=="FYS") return fs.FYS;
                                    if (bs=="SMI") return fs.SMI;
                                    if (bs=="INT") return fs.INT;
                                    if (bs=="PSY") return fs.PSY;
                                    if (bs=="KAR") return fs.KAR;
                                    if (bs=="STO") return fs.STO;
                                    return 10;
                                }() : 10);
                                e.fv      = bc;
                                e.fvBase  = bc;
                                e.clNote  = note;
                                e.isPrimary         = isPrim;
                                e.isProfessionSkill = false;
                                state.character.skills.push_back(e);
                            }
                        }
                    }
                }
                // If the ability grants a weapon-hand dominance, lock it immediately
                {
                    bool grantsAmbi   = resultStr->find("Ambidextrous") != std::string::npos;
                    bool grantsDouble = resultStr->find("Double handed") != std::string::npos;
                    if (grantsAmbi || grantsDouble) {
                        state.character.weaponHand                   = grantsAmbi ? "Ambidextrous" : "Double handed";
                        state.character.weaponHandRoll               = 0;
                        state.character.weaponHandFromSpecialAbility = true;
                        state.markComplete(Section::WeaponHand);
                    }
                }
                break;
            case DiceContext::WeaponHand:
                state.character.weaponHand     = *resultStr;
                state.character.weaponHandRoll = result.total;
                state.bpBreakdown.forWeaponHand = bp;
                state.markComplete(Section::WeaponHand);
                break;
            case DiceContext::Social:
                state.character.socialDescription = *resultStr;
                state.character.socialRoll        = result.total;
                state.bpBreakdown.forSocial       = bp;
                state.socialBPSpent               = bp;
                state.markComplete(Section::Social);
                break;
            case DiceContext::Capital: {
                int base = GameRules::getStartingCapitalBase(result.total);
                state.character.capitalBase       = base;
                state.character.startingCapital   = base;  // age multiplier applied in AgeScreen
                state.bpBreakdown.forCapital      = bp;
                state.markComplete(Section::Capital);
                // Update result string to show base capital
                *resultStr = std::to_string(base) + " sm (base, age multiplier applied later)";
                break;
            }
        }
    };

    auto skipRoll = [&app, ctx, hasRolled]() {
        auto& state = app.state();
        if (ctx == DiceContext::SpecialAbility) {
            state.character.hasSpecialAbility = false;
            state.markComplete(Section::SpecialAbility);
            *hasRolled = true;
            app.navigate(Section::Dashboard);
        }
    };

    auto renderer = Renderer(layout, [&app, ctx, meta, bpBonus, hasRolled, rollTotal, rollDice,
                                       resultStr, statusMsg, layout]() -> Element {
        auto& state = app.state();
        *meta = getCtxMeta(ctx, state);  // refresh maxBP

        Elements descLines;
        std::istringstream ss(meta->description);
        std::string line;
        while (std::getline(ss, line)) {
            descLines.push_back(text("  " + line) | color(kDim));
        }

        Element rollSection;
        if (*hasRolled) {
            // Compute modifier = total − sum of individual dice
            int diceSum = 0;
            for (int d : *rollDice) diceSum += d;
            int rollMod = rollDice->empty() ? 0 : (*rollTotal - diceSum);

            Elements calcEls;
            if (rollDice->empty()) {
                calcEls.push_back(text("(result restored from save)") | color(kDim));
            } else {
                for (int d : *rollDice)
                    calcEls.push_back(text(" [" + std::to_string(d) + "] ") | bold | color(kTitle));
                if (rollMod != 0)
                    calcEls.push_back(text((rollMod >= 0 ? " +" : " ") + std::to_string(rollMod)) | color(kAccent));
                calcEls.push_back(text(" = " + std::to_string(*rollTotal)) | bold | color(kAccent));
            }

            rollSection = vbox({
                hbox({text("  Roll:   ") | color(kDim), hbox(calcEls)}),
                text("  Result: ") | color(kHeader),
                paragraph("          " + *resultStr) | bold | color(kSuccess),
                text(""),
                text("  ✓ Roll is locked.") | color(kSuccess) | bold,
            });
        } else {
            rollSection = vbox({
                hbox({
                    text("  BP bonus to spend: ") | color(kDim),
                    text("← ") | color(kAccent),
                    text(std::to_string(*bpBonus)) | bold | color(kTitle) | size(WIDTH, EQUAL, 4),
                    text(" →") | color(kAccent),
                    text("  (0 – " + std::to_string(meta->maxBP) + ", available: "
                         + std::to_string(state.bpRemaining()) + ")") | color(kDim),
                }),
                text(""),
                keyBar({{"←/→","Adjust BP bonus"}, {"R","Roll the dice!"}, {"S","Skip (special ability only)"}, {"Esc","Back"}}),
                text(""),
                statusMsg->empty() ? text("") : text("  " + *statusMsg) | color(kError),
            });
        }

        return vbox({
            headerBar(meta->title),
            vbox({
                text(""),
                vbox(descLines),
                text(""),
                separator() | color(kDim),
                text(""),
                rollSection,
                filler(),
                bpBar(state.bpRemaining(), 125),
            }) | flex ,
        }) | border;
    });

    return CatchEvent(renderer, [&app, doRoll, skipRoll, hasRolled, bpBonus, meta](Event e) -> bool {
        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (!*hasRolled) {
            if (e == Event::ArrowLeft) {
                if (*bpBonus > 0) { --(*bpBonus); }
                return true;
            }
            if (e == Event::ArrowRight) {
                auto& state = app.state();
                int cap = std::min(meta->maxBP, state.bpRemaining());
                if (*bpBonus < cap) { ++(*bpBonus); }
                return true;
            }
            if (e == Event::Character('r') || e == Event::Character('R')) {
                doRoll();
                return true;
            }
            if (e == Event::Character('s') || e == Event::Character('S')) {
                skipRoll();
                return true;
            }
        }
        if (*hasRolled && e == Event::Return) {
            app.navigate(Section::Dashboard);
            return true;
        }
        return false;
    });
}

} // namespace dod
