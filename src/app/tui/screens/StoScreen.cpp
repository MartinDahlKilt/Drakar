#include "StoScreen.h"
#include "../TuiTheme.h"

#include "core/GameData.h"
#include "core/GameRules.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

namespace dod {

ftxui::Component MakeStoScreen(TuiApp& app) {
    using namespace theme;

    auto statusMsg = std::make_shared<std::string>("");

    auto dummyComp = Container::Vertical({});

    auto adjustSTO = [&app, statusMsg](int delta) {
        auto& state = app.state();
        if (state.character.race.empty()) {
            *statusMsg = "Select a race first.";
            return;
        }
        auto raceOpt = GameData::findRace(state.character.race);
        if (!raceOpt) return;

        int newSto = state.character.purchasedStats.STO + delta;
        if (newSto < raceOpt->stoRange.minVal || newSto > raceOpt->stoRange.maxVal) {
            *statusMsg = "STO out of range (" + std::to_string(raceOpt->stoRange.minVal)
                       + "–" + std::to_string(raceOpt->stoRange.maxVal) + ").";
            return;
        }

        int curAdj = state.character.stoAdjustment;
        int newAdj = curAdj + delta;
        int oldCost = GameRules::getSTOAdjustmentCost(curAdj);
        int newCost = GameRules::getSTOAdjustmentCost(newAdj);
        int diff    = newCost - oldCost;

        if (diff > 0 && diff > state.bpRemaining()) {
            *statusMsg = "Not enough BP!";
            return;
        }

        state.bpBreakdown.forSTO         += diff;
        state.character.purchasedStats.STO = newSto;
        state.character.stoAdjustment      = newAdj;
        *statusMsg = "";
    };

    auto renderer = Renderer(dummyComp, [&app, statusMsg]() -> Element {
        auto& state = app.state();
        int sto    = state.character.purchasedStats.STO;
        int adj    = state.character.stoAdjustment;
        int normal = state.character.stoNormal;

        int minVal = normal, maxVal = normal;
        if (!state.character.race.empty()) {
            auto raceOpt = GameData::findRace(state.character.race);
            if (raceOpt) {
                minVal = raceOpt->stoRange.minVal;
                maxVal = raceOpt->stoRange.maxVal;
            }
        }

        std::string adjStr = (adj == 0) ? "0 (no cost)"
            : (adj > 0 ? "+" : "") + std::to_string(adj)
              + "  (" + std::to_string(std::abs(adj) * 2) + " BP "
              + (adj > 0 ? "spent" : "refunded") + ")";

        return vbox({
            headerBar("Size (STO)"),
            vbox({
                text("") ,
                text("  STO determines body size, affects HP and damage bonus.") | color(kDim),
                text("  Adjusting STO costs/refunds 2 BP per step from race normal.") | color(kDim),
                text(""),
                hbox({
                    vbox({
                        labelValue("Race:        ", state.character.race.empty() ? "—" : state.character.race),
                        labelValue("Race normal: ", std::to_string(normal)),
                        labelValue("Range:       ", std::to_string(minVal) + " – " + std::to_string(maxVal)),
                        text(""),
                        hbox({
                            text("  Current STO:  ") | color(kDim),
                            text(std::to_string(sto)) | bold | color(kTitle) | size(WIDTH, EQUAL, 4),
                            text("  ← / → to adjust"),
                        }),
                        text("  Adjustment:   " + adjStr) | color(adj == 0 ? kDim : kAccent),
                        text(""),
                        text("  BP for STO:   " + std::to_string(state.bpBreakdown.forSTO)) | color(kBPColor),
                    }) | flex,
                    vbox({
                        text("Controls") | bold | color(kHeader),
                        separator() | color(kDim),
                        text("  ←  Decrease STO (refund BP)") | color(kDim),
                        text("  →  Increase STO (spend BP)") | color(kDim),
                        text("  Enter / Esc  Done / Back") | color(kDim),
                    }) | size(WIDTH, EQUAL, 35),
                }),
                text(""),
                statusMsg->empty() ? text("") : text("  " + *statusMsg) | color(kError),
                filler(),
                bpBar(state.bpRemaining(), state.bpTotal()),
            }) | flex ,
        }) | border;
    });

    return CatchEvent(renderer, [&app, adjustSTO](Event e) -> bool {
        auto& state = app.state();
        if (e == Event::Escape || e == Event::Return) {
            state.markComplete(Section::STO);
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
        if (e == Event::ArrowLeft)  { adjustSTO(-1); return true; }
        if (e == Event::ArrowRight) { adjustSTO(+1); return true; }
        return false;
    });
}

} // namespace dod
