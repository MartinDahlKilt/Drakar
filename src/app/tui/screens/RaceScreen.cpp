#include "RaceScreen.h"
#include "../TuiTheme.h"

#include "core/GameData.h"
#include "core/GameRules.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <cmath>

using namespace ftxui;

namespace dod {

ftxui::Component MakeRaceScreen(TuiApp& app) {
    using namespace theme;

    const auto& allRaces = GameData::getRaces();

    // Build a filtered index list; Anka is excluded unless the setting is enabled.
    auto filteredIdx = std::make_shared<std::vector<int>>();
    for (int i = 0; i < (int)allRaces.size(); ++i) {
        if (allRaces[i].name == "Anka" && !app.state().allowAnka) continue;
        filteredIdx->push_back(i);
    }

    auto selIdx = std::make_shared<int>(0);
    {
        auto& state = app.state();
        for (int fi = 0; fi < (int)filteredIdx->size(); ++fi) {
            if (allRaces[(*filteredIdx)[fi]].name == state.character.race) {
                *selIdx = fi;
                break;
            }
        }
    }

    // Confirmation dialog for stat refund
    auto showConfirm = std::make_shared<bool>(false);
    auto statusMsg   = std::make_shared<std::string>("");

    auto raceNames = std::make_shared<std::vector<std::string>>();
    for (int i : *filteredIdx) raceNames->push_back(allRaces[i].name);
    MenuOption raceMenuOpt;
    raceMenuOpt.focused_entry = selIdx.get();
    auto menu = Menu(raceNames.get(), selIdx.get(), raceMenuOpt);

    auto applyRace = [&app, selIdx, statusMsg, filteredIdx]() {
        auto& state = app.state();
        const auto& allR = GameData::getRaces();
        const auto& r = allR[(*filteredIdx)[*selIdx]];
        // Pay race cost
        state.bpBreakdown.forRace = r.bpCost;
        state.character.race      = r.name;
        state.character.raceMods  = {
            r.statMods[0], r.statMods[1], r.statMods[2],
            r.statMods[3], r.statMods[4], r.statMods[5], 0
        };
        state.character.stoNormal            = r.stoRange.normal;
        state.character.purchasedStats.STO   = r.stoRange.normal;
        state.character.stoAdjustment        = 0;
        state.markComplete(Section::Race);
        *statusMsg = "Race set to " + r.name + ".";
    };

    auto trySelectRace = [&app, selIdx, showConfirm, applyRace, filteredIdx]() {
        auto& state = app.state();
        const auto& allR = GameData::getRaces();
        bool statsAlreadyBought = state.bpBreakdown.forStats > 0;
        bool raceBought         = state.bpBreakdown.forRace  > 0;
        // Changing race always requires stat refund
        if ((statsAlreadyBought || raceBought) && state.character.race != allR[(*filteredIdx)[*selIdx]].name) {
            *showConfirm = true;
        } else {
            applyRace();
            app.navigate(Section::Dashboard);
        }
    };

    auto doConfirmYes = [&app, applyRace, showConfirm]() {
        app.state().refundRace();   // refunds race BP + stat BP
        applyRace();
        *showConfirm = false;
        app.navigate(Section::Dashboard);
    };

    auto renderer = Renderer(menu, [&app, selIdx, showConfirm, statusMsg, menu, raceNames, filteredIdx]() -> Element {
        auto& state = app.state();
        const auto& allR = GameData::getRaces();
        const auto& race = allR[(*filteredIdx)[*selIdx]];

        // Stat mod list
        Elements modLines;
        const char* statNames[] = {"STY","FYS","SMI","INT","PSY","KAR","STO"};
        for (int i = 0; i < 6; ++i) {
            if (race.statMods[i] != 0) {
                modLines.push_back(hbox({
                    text("  " + std::string(statNames[i]) + "  "),
                    text((race.statMods[i] > 0 ? "+" : "") + std::to_string(race.statMods[i]))
                        | bold | color(race.statMods[i] > 0 ? kSuccess : kError),
                }));
            }
        }
        if (modLines.empty()) modLines.push_back(text("  None") | color(kDim));

        // Special bonuses
        Elements bonusLines;
        for (auto& b : race.specialBonuses) {
            std::string txt = "  " + std::string(skillIdToString(b.skillId)) + ": ";
            txt += b.setFixed ? "FV fixed at " + std::to_string(b.fvBonus)
                              : "+" + std::to_string(b.fvBonus) + " FV";
            bonusLines.push_back(text(txt) | color(kDim));
        }
        if (bonusLines.empty()) bonusLines.push_back(text("  None") | color(kDim));

        Element rightPanel = vbox({
            text(race.name) | bold | color(kTitle),
            separator() | color(kDim),
            labelValue("BP cost: ", std::to_string(race.bpCost) + " BP",
                       state.bpRemaining() >= race.bpCost ? kSuccess : kError),
            text(""),
            text("STO range:") | bold | color(kHeader),
            text("  " + std::to_string(race.stoRange.minVal)
               + " – " + std::to_string(race.stoRange.maxVal)
               + "  (normal: " + std::to_string(race.stoRange.normal) + ")") | color(kDim),
            text(""),
            text("Stat modifiers:") | bold | color(kHeader),
            vbox(modLines),
            text(""),
            text("Special bonuses:") | bold | color(kHeader),
            vbox(bonusLines),
            text(""),
            paragraph(race.description) | color(kDim),
            filler(),
            separator() | color(kDim),
            statusMsg->empty() ? text("") : text(*statusMsg) | color(kAccent),
            text(""),
            keyBar({{"Enter","Select race"}, {"Esc","Back"}}),
        });

        Element confirmOverlay = vbox({
            text("") ,
            vbox({
                text(" Changing race will refund and reset your stat purchases.") | color(kWarning),
                text(" Continue?") | bold,
                text(""),
                keyBar({{"Y","Yes, change race"}, {"N","Cancel"}}),
            }) | border | color(kWarning),
        });

        Element body = vbox({
            headerBar("Race"),
            hbox({
                vbox({
                    text(" Select Race") | bold | color(kHeader),
                    separator() | color(kDim),
                    menu->Render() | flex,
                }) | size(WIDTH, EQUAL, 22),
                separator() | color(kDim),
                rightPanel | flex ,
            }) | flex,
            bpBar(state.bpRemaining(), 125),
        }) | border;

        if (*showConfirm) {
            return dbox({body, confirmOverlay | center | flex});
        }
        return body;
    });

    return CatchEvent(renderer, [&app, menu, trySelectRace, doConfirmYes, showConfirm, raceNames](Event e) -> bool {
        if (*showConfirm) {
            if (e == Event::Character('y') || e == Event::Character('Y')) {
                doConfirmYes();
                return true;
            }
            if (e == Event::Character('n') || e == Event::Character('N') || e == Event::Escape) {
                *showConfirm = false;
                return true;
            }
            return true;
        }
        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::Return) {
            trySelectRace();
            return true;
        }
        return menu->OnEvent(e);
    });
}

} // namespace dod
