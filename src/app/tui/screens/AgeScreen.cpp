#include "AgeScreen.h"
#include "../TuiTheme.h"

#include "core/GameData.h"
#include "core/GameRules.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <sstream>

using namespace ftxui;

namespace dod {

ftxui::Component MakeAgeScreen(TuiApp& app) {
    using namespace theme;

    const auto& ages = GameData::getAgeCategories();

    auto selIdx = std::make_shared<int>(0);
    {
        auto& state = app.state();
        for (int i = 0; i < (int)ages.size(); ++i) {
            if (ages[i].name == state.character.ageCategory) {
                *selIdx = i;
                break;
            }
        }
    }

    auto ageNames = std::make_shared<std::vector<std::string>>();
    for (auto& a : ages) ageNames->push_back(a.name);
    MenuOption ageMenuOpt;
    ageMenuOpt.focused_entry = selIdx.get();
    auto menu = Menu(ageNames.get(), selIdx.get(), ageMenuOpt);

    auto renderer = Renderer(menu, [&app, selIdx, menu, ageNames]() -> Element {
        auto& state = app.state();
        const auto& ages = GameData::getAgeCategories();
        const auto& age = ages[*selIdx];
        const char* snames[] = {"STY","FYS","SMI","INT","PSY","KAR","STO"};
        Elements modLines;
        for (int i = 0; i < 7; ++i) {
            if (age.statMods[i] != 0) {
                modLines.push_back(hbox({
                    text("  " + std::string(snames[i]) + "  "),
                    text((age.statMods[i] > 0 ? "+" : "") + std::to_string(age.statMods[i]))
                        | bold | color(age.statMods[i] > 0 ? kSuccess : kError),
                }));
            }
        }
        if (modLines.empty()) modLines.push_back(text("  None") | color(kDim));

        // Compute EP preview
        int epFromAge = age.ep;
        int epFromBP  = state.bpRemaining() * 5;
        // Capital preview
        int capBase = state.character.capitalBase > 0 ? state.character.capitalBase : state.character.startingCapital;
        std::string capStr = "Base " + std::to_string(capBase)
                           + " sm × " + std::to_string((int)(age.capitalMultiplier * 10)) + "/10";
        int capFinal = GameRules::calculateStartingCapital(capBase, age.capitalMultiplier);
        // Check for stats that would drop to zero or below with this age
        const auto curStats = state.computeFinalStats();
        int previewArr[7] = {
            curStats.STY + age.statMods[0],
            curStats.FYS + age.statMods[1],
            curStats.SMI + age.statMods[2],
            curStats.INT + age.statMods[3],
            curStats.PSY + age.statMods[4],
            curStats.KAR + age.statMods[5],
            curStats.STO + age.statMods[6],
        };
        Elements zeroWarnings;
        for (int i = 0; i < 7; ++i) {
            if (previewArr[i] <= 0) {
                zeroWarnings.push_back(
                    text("  \u26a0 " + std::string(snames[i]) + " would be "
                         + std::to_string(previewArr[i]) + " \u2014 cannot confirm!")
                    | bold | color(kError)
                );
            }
        }
        Element rightPanel = vbox({
            text(age.name) | bold | color(kTitle),
            separator() | color(kDim),
            text(age.ageRangeNote) | color(kDim),
            text(""),
            text("Stat modifiers:") | bold | color(kHeader),
            vbox(modLines),
            text(""),
            labelValue("EP from age:       ", std::to_string(epFromAge), kAccent),
            labelValue("EP from unspent BP: ", std::to_string(state.bpRemaining()) + " × 5 = "
                                              + std::to_string(epFromBP), kAccent),
            labelValue("Total EP:           ", std::to_string(epFromAge + epFromBP), kSuccess),
            text(""),
            labelValue("Capital multiplier: ", std::to_string(age.capitalMultiplier).substr(0,4) + "×", kAccent),
            labelValue("Starting capital:   ", capStr + " = " + std::to_string(capFinal) + " sm", kSuccess),
            text(""),
            labelValue("Max start FV:       ", std::to_string(age.maxStartFV), kDim),
            text(""),
            vbox(zeroWarnings),
            filler(),
            separator() | color(kDim),
            keyBar({{"Enter","Confirm age"}, {"Esc","Back"}}),
        });

        return vbox({
            headerBar("Age Category"),
            hbox({
                vbox({
                    text(" Select Age Category") | bold | color(kHeader),
                    separator() | color(kDim),
                    menu->Render() | flex,
                }) | size(WIDTH, EQUAL, 28),
                separator() | color(kDim),
                rightPanel | flex ,
            }) | flex,
            bpBar(state.bpRemaining(), 125),
        }) | border;
    });

    return CatchEvent(renderer, [&app, selIdx, menu, ageNames](Event e) -> bool {
        auto& state = app.state();
        const auto& ages = GameData::getAgeCategories();
        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::Return) {
            const auto& age = ages[*selIdx];

            state.character.ageCategory = age.name;

            // Apply age mods
            state.character.ageMods.STY = age.statMods[0];
            state.character.ageMods.FYS = age.statMods[1];
            state.character.ageMods.SMI = age.statMods[2];
            state.character.ageMods.INT = age.statMods[3];
            state.character.ageMods.PSY = age.statMods[4];
            state.character.ageMods.KAR = age.statMods[5];
            state.character.ageMods.STO = age.statMods[6];

            // Recompute finals
            state.character.finalStats = state.computeFinalStats();
            const auto& f = state.character.finalStats;

            // Derived stats
            state.character.derived.hpTotal   = GameRules::calculateHP(f.FYS, f.STO);
            state.character.derived.hpParts   = GameRules::calculateBodyPartHP(state.character.derived.hpTotal);
            state.character.derived.damageBonus = GameRules::calculateDamageBonus(f.STY, f.STO);
            state.character.derived.movement  = GameRules::calculateMovement(f.STO, f.FYS, f.SMI, state.character.race);

            // Capital (only multiply if capital was already rolled)
            if (state.isComplete(Section::Capital)) {
                int base = state.character.capitalBase > 0 ? state.character.capitalBase : state.character.startingCapital;
                state.character.startingCapital = GameRules::calculateStartingCapital(
                    base, age.capitalMultiplier);
            }

            // EP
            state.character.epFromAge = age.ep;
            state.character.epFromBP  = state.bpRemaining() * 5;
            state.character.epTotal   = state.character.epFromAge + state.character.epFromBP;

            state.markComplete(Section::Age);
            app.navigate(Section::Dashboard);
            return true;
        }
        return menu->OnEvent(e);
    });
}

} // namespace dod
