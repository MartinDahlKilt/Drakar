#include "StatsScreen.h"
#include "../TuiTheme.h"

#include "core/GameData.h"
#include "core/GameRules.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <algorithm>
#include <sstream>

using namespace ftxui;

namespace dod {

namespace {

struct StatRow {
    const char* name;
    int* purchased;  // pointer into purchasedStats
};

int getStatVal(const Stats& s, const std::string& n) {
    if (n == "STY") return s.STY;
    if (n == "FYS") return s.FYS;
    if (n == "SMI") return s.SMI;
    if (n == "INT") return s.INT;
    if (n == "PSY") return s.PSY;
    if (n == "KAR") return s.KAR;
    return 0;
}

void setStatVal(Stats& s, const std::string& n, int v) {
    if (n == "STY") s.STY = v;
    else if (n == "FYS") s.FYS = v;
    else if (n == "SMI") s.SMI = v;
    else if (n == "INT") s.INT = v;
    else if (n == "PSY") s.PSY = v;
    else if (n == "KAR") s.KAR = v;
}

// Recompute forStats from all 6 purchased stat costs
int computeStatsBPTotal(const Stats& ps) {
    return GameRules::getStatBPCost(ps.STY)
         + GameRules::getStatBPCost(ps.FYS)
         + GameRules::getStatBPCost(ps.SMI)
         + GameRules::getStatBPCost(ps.INT)
         + GameRules::getStatBPCost(ps.PSY)
         + GameRules::getStatBPCost(ps.KAR);
}

} // namespace

ftxui::Component MakeStatsScreen(TuiApp& app) {
    using namespace theme;

    static const std::vector<std::string> kStatNames = {"STY","FYS","SMI","INT","PSY","KAR"};
    auto focusRow  = std::make_shared<int>(0);
    auto statusMsg = std::make_shared<std::string>("");

    // Ensure purchased stats are at least 3
    {
        auto& ps = app.state().character.purchasedStats;
        if (ps.STY < 3) ps.STY = 3;
        if (ps.FYS < 3) ps.FYS = 3;
        if (ps.SMI < 3) ps.SMI = 3;
        if (ps.INT < 3) ps.INT = 3;
        if (ps.PSY < 3) ps.PSY = 3;
        if (ps.KAR < 3) ps.KAR = 3;
    }

    auto adjustStat = [&app, focusRow, statusMsg](int delta) {
        auto& state = app.state();
        const std::string& statName = kStatNames[*focusRow];
        auto& ps2 = state.character.purchasedStats;
        int cur = getStatVal(ps2, statName);
        int nxt = cur + delta;
        if (nxt < 3 || nxt > 18) return;

        int oldCost = computeStatsBPTotal(ps2);
        int newCost = oldCost - GameRules::getStatBPCost(cur) + GameRules::getStatBPCost(nxt);
        int otherBP = state.bpBreakdown.total() - state.bpBreakdown.forStats;
        if (newCost + otherBP > 125) {
            *statusMsg = "Not enough BP!";
            return;
        }

        setStatVal(ps2, statName, nxt);
        state.bpBreakdown.forStats = computeStatsBPTotal(ps2);
        *statusMsg = "";
    };

    auto autoFill = [&app, statusMsg]() {
        auto& state = app.state();
        if (state.character.profession.empty()) {
            *statusMsg = "Select a profession first to auto-fill.";
            return;
        }
        auto profOpt = GameData::findProfession(state.character.profession);
        if (!profOpt || profOpt->statRequirements.empty()) {
            *statusMsg = "No requirements for " + state.character.profession + ".";
            return;
        }
        auto& ps2 = state.character.purchasedStats;
        auto& rm  = state.character.raceMods;
        int extraBP = 0;
        for (const auto& [sn, minFinal] : profOpt->statRequirements) {
            int raceBonus = getStatVal(rm, sn);
            int needed    = std::max(3, minFinal - raceBonus);
            int cur       = getStatVal(ps2, sn);
            if (needed > cur)
                extraBP += GameRules::getStatRaiseCost(cur, needed);
        }
        int otherBP = state.bpBreakdown.total() - state.bpBreakdown.forStats;
        int curStatCost = computeStatsBPTotal(ps2);
        if (curStatCost + extraBP + otherBP > 125) {
            *statusMsg = "Not enough BP (" + std::to_string(extraBP) + " extra needed).";
            return;
        }
        for (const auto& [sn, minFinal] : profOpt->statRequirements) {
            int raceBonus = getStatVal(rm, sn);
            int needed    = std::max(3, minFinal - raceBonus);
            int cur       = getStatVal(ps2, sn);
            if (needed > cur) setStatVal(ps2, sn, needed);
        }
        state.bpBreakdown.forStats = computeStatsBPTotal(ps2);
        *statusMsg = "Auto-filled for " + state.character.profession + "!";
    };

    auto dummyComp = Container::Vertical({});

    auto renderer = Renderer(dummyComp, [&app, focusRow, statusMsg]() -> Element {
        auto& state = app.state();
        const auto& ps2  = state.character.purchasedStats;
        const auto& rm   = state.character.raceMods;
        const auto& am   = state.character.ageMods;
        auto finalStats  = state.computeFinalStats();

        std::optional<ProfessionDefinition> profOpt;
        if (!state.character.profession.empty())
            profOpt = GameData::findProfession(state.character.profession);

        // Fixed column widths for perfect alignment
        // ind  name  bought  race  age  final  req  cost
        //  2    5      7      7     7     7      6    6
        auto col = [](Element e, int w) {
            return e | size(WIDTH, EQUAL, w);
        };

        Elements rows;
        rows.push_back(hbox({
            col(text(""),                          2),
            col(text("Stat") | bold,               5),
            col(text("Bought") | bold | color(kHeader),  7),
            col(text("Race")   | bold | color(kDim),     7),
            col(text("Age")    | bold | color(kDim),     7),
            col(text("Final")  | bold | color(kTitle),   7),
            col(text("BC")     | bold | color(kAccent),  5),
            col(text("Req")    | bold | color(kWarning), 6),
            col(text("Cost")   | bold | color(kBPColor), 6),
        }));
        rows.push_back(separator() | color(kDim));

        for (int i = 0; i < 6; ++i) {
            const std::string& sn = kStatNames[i];
            int bought  = getStatVal(ps2, sn);
            int race    = getStatVal(rm, sn);
            int age     = getStatVal(am, sn);
            int final_  = getStatVal(finalStats, sn);
            int bc      = GameRules::calculateBC(final_);
            int cost    = GameRules::getStatBPCost(bought);

            int req = 0;
            if (profOpt) {
                auto it = profOpt->statRequirements.find(sn);
                if (it != profOpt->statRequirements.end()) req = it->second;
            }

            bool reqMet  = (req == 0) || (final_ >= req);
            bool focused = (i == *focusRow);

            std::string ageStr  = (age  != 0 ? (age  > 0 ? "+" : "") + std::to_string(age)  : "--");
            std::string raceStr = (race != 0 ? (race > 0 ? "+" : "") + std::to_string(race) : "--");
            std::string reqStr  = (req  >  0 ? std::to_string(req) : "--");

            Element row = hbox({
                col(text(focused ? "►" : " "),                                          2),
                col(text(sn) | bold,                                                    5),
                col(text(std::to_string(bought)) | bold | color(kValue),                7),
                col(text(raceStr) | color(race != 0 ? (race > 0 ? kSuccess : kError) : kDim), 7),
                col(text(ageStr)  | color(age  != 0 ? (age  > 0 ? kSuccess : kError) : kDim), 7),
                col(text(std::to_string(final_)) | bold | color(reqMet ? kSuccess : kError),  7),
                col(text(std::to_string(bc)) | color(kAccent),                          5),
                col(text(reqStr) | color(req > 0 ? kWarning : kDim),                   6),
                col(text(std::to_string(cost)) | color(kBPColor),                      6),
            });

            if (focused) row = row | bgcolor(Color::Blue);
            rows.push_back(row);
        }

        Element instructions = vbox({
            text("Controls") | bold | color(kHeader),
            separator() | color(kDim),
            text("  ↑/↓  Change focused stat") | color(kDim),
            text("  ←/→  Decrease / increase value") | color(kDim),
            text("  A    Auto-fill requirements") | color(kDim),
            text("  Esc  Done / back to dashboard") | color(kDim),
            filler(),
            separator() | color(kDim),
            bpBar(state.bpRemaining(), 125),
            text(""),
            statusMsg->empty() ? text("") : text(*statusMsg) | color(kAccent),
        });

        return vbox({
            headerBar("Statistics"),
            hbox({
                vbox({
                    text("") ,
                    text("  Use ↑/↓ to select stat, ←/→ to adjust") | color(kDim),
                    text(""),
                    vbox(rows),
                    text(""),
                    text("  Total stat cost: " + std::to_string(state.bpBreakdown.forStats) + " BP") | color(kBPColor),
                }) | flex,
                separator() | color(kDim),
                instructions | size(WIDTH, EQUAL, 35) ,
            }) | flex,
        }) | border;
    });

    return CatchEvent(renderer, [&app, focusRow, adjustStat, autoFill](Event e) -> bool {
        auto& state = app.state();
        if (e == Event::Escape) {
            state.markComplete(Section::Stats);
            {
                const auto& f = state.character.finalStats = state.computeFinalStats();
                state.character.derived.hpTotal     = GameRules::calculateHP(f.FYS, f.STO);
                state.character.derived.hpParts     = GameRules::calculateBodyPartHP(state.character.derived.hpTotal);
                state.character.derived.damageBonus = GameRules::calculateDamageBonus(f.STY, f.STO);
                state.character.derived.movement    = GameRules::calculateMovement(f.STO, f.FYS, f.SMI, state.character.race);
            }
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::Return) {
            state.markComplete(Section::Stats);
            {
                const auto& f = state.character.finalStats = state.computeFinalStats();
                state.character.derived.hpTotal     = GameRules::calculateHP(f.FYS, f.STO);
                state.character.derived.hpParts     = GameRules::calculateBodyPartHP(state.character.derived.hpTotal);
                state.character.derived.damageBonus = GameRules::calculateDamageBonus(f.STY, f.STO);
                state.character.derived.movement    = GameRules::calculateMovement(f.STO, f.FYS, f.SMI, state.character.race);
            }
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::ArrowUp) {
            *focusRow = (*focusRow - 1 + 6) % 6;
            return true;
        }
        if (e == Event::ArrowDown) {
            *focusRow = (*focusRow + 1) % 6;
            return true;
        }
        if (e == Event::ArrowLeft)  { adjustStat(-1); return true; }
        if (e == Event::ArrowRight) { adjustStat(+1); return true; }
        if (e == Event::Character('a') || e == Event::Character('A')) {
            autoFill();
            return true;
        }
        return false;
    });
}

} // namespace dod
