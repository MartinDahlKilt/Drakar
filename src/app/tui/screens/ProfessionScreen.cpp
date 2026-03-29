#include "ProfessionScreen.h"
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

static int getStatVal(const Stats& s, const std::string& name) {
    if (name == "STY") return s.STY;
    if (name == "FYS") return s.FYS;
    if (name == "SMI") return s.SMI;
    if (name == "INT") return s.INT;
    if (name == "PSY") return s.PSY;
    if (name == "KAR") return s.KAR;
    if (name == "STO") return s.STO;
    return 0;
}

ftxui::Component MakeProfessionScreen(TuiApp& app) {
    using namespace theme;

    const auto& profs = GameData::getProfessions();

    auto profMenu = std::make_shared<int>(0);
    // Pre-select current profession if any
    {
        auto& state = app.state();
        for (int i = 0; i < (int)profs.size(); ++i) {
            if (profs[i].name == state.character.profession) {
                *profMenu = i;
                break;
            }
        }
    }

    auto statusMsg = std::make_shared<std::string>("");

    auto profNames = std::make_shared<std::vector<std::string>>();
    for (auto& p : profs) profNames->push_back(p.name);
    MenuOption profMenuOpt;
    profMenuOpt.focused_entry = profMenu.get();
    auto menu = Menu(profNames.get(), profMenu.get(), profMenuOpt);

    auto renderer = Renderer(menu, [&app, profMenu, statusMsg, menu, profNames]() -> Element {
        auto& state = app.state();
        const auto& profs = GameData::getProfessions();
        const auto& prof  = profs[*profMenu];
        auto  finalStats  = state.computeFinalStats();

        // Build requirements section
        Elements reqLines;
        if (prof.statRequirements.empty()) {
            reqLines.push_back(text("  No requirements") | color(kDim));
        } else {
            for (const auto& [stat, minVal] : prof.statRequirements) {
                int cur = getStatVal(finalStats, stat);
                bool ok = (cur >= minVal);
                reqLines.push_back(hbox({
                    text("  " + stat + " ≥ " + std::to_string(minVal)),
                    text("  (now " + std::to_string(cur) + ")") | color(ok ? kSuccess : kError) | bold,
                }));
            }
        }

        // Build skill count info
        std::string skillInfo = "Skill slots: " + std::to_string(prof.maxSkillChoices);
        if (prof.maxWeaponSkills >= 0)
            skillInfo += "  Weapons: max " + std::to_string(prof.maxWeaponSkills);

        Element rightPanel = vbox({
            text(prof.name) | bold | color(kTitle),
            separator() | color(kDim),
            text("Description:") | bold | color(kHeader),
            paragraph(prof.description) | color(kDim),
            text(""),
            text("Special ability:") | bold | color(kHeader),
            paragraph(prof.abilityDescription) | color(kDim),
            text(""),
            text("Requirements:") | bold | color(kHeader),
            vbox(reqLines),
            text(""),
            text(skillInfo) | color(kDim),
            filler(),
            separator() | color(kDim),
            statusMsg->empty() ? text("") : text(*statusMsg) | color(kAccent),
            text(""),
            keyBar({{"Enter","Select profession"},{"Esc","Back"}}),
        });

        return vbox({
            headerBar("Profession"),
            hbox({
                vbox({
                    text(" Select Profession") | bold | color(kHeader),
                    separator() | color(kDim),
                    menu->Render() | flex,
                }) | size(WIDTH, EQUAL, 25),
                separator() | color(kDim),
                rightPanel | flex ,
            }) | flex,
            bpBar(state.bpRemaining(), 125),
        }) | border;
    });

    return CatchEvent(renderer, [&app, profMenu, menu, statusMsg, profNames](Event e) -> bool {
        auto& state = app.state();
        const auto& profs = GameData::getProfessions();
        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::Return) {
            state.character.profession = profs[*profMenu].name;
            state.markComplete(Section::Profession);
            *statusMsg = "";
            app.navigate(Section::Dashboard);
            return true;
        }
        return menu->OnEvent(e);
    });
}

} // namespace dod
