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

    // Filtered index — pointing into the *currently visible* list
    auto profMenu  = std::make_shared<int>(0);
    auto statusMsg = std::make_shared<std::string>("");

    // Shared mutable names vector that the Menu widget reads from
    auto profNames = std::make_shared<std::vector<std::string>>();

    // Helper: rebuild the filtered list and return it alongside the original indices
    auto buildFiltered = [](const TuiState& state)
            -> std::vector<std::pair<int, const ProfessionDefinition*>>
    {
        const auto& all = GameData::getProfessions();
        std::vector<std::pair<int, const ProfessionDefinition*>> result;
        for (int i = 0; i < (int)all.size(); ++i) {
            const auto& p = all[i];
            if (state.allowWarriorExpansion) {
                // Hide base Krigare and Riddare; show warrior expansion professions
                if (!p.isWarriorExpansion
                    && (p.name == "Krigare" || p.name == "Riddare")) continue;
            } else {
                // Hide warrior expansion professions
                if (p.isWarriorExpansion) continue;
            }
            result.push_back({i, &p});
        }
        return result;
    };

    // Pre-select the currently chosen profession in the filtered list
    {
        auto& state = app.state();
        auto filtered = buildFiltered(state);
        for (int i = 0; i < (int)filtered.size(); ++i) {
            if (filtered[i].second->name == state.character.profession) {
                *profMenu = i;
                break;
            }
        }
        for (auto& [idx, p] : filtered) profNames->push_back(p->name);
    }

    MenuOption profMenuOpt;
    profMenuOpt.focused_entry = profMenu.get();
    auto menu = Menu(profNames.get(), profMenu.get(), profMenuOpt);

    auto renderer = Renderer(menu, [&app, profMenu, statusMsg, menu, profNames, buildFiltered]() -> Element {
        auto& state = app.state();

        // Rebuild filtered list so it stays up-to-date with expansion toggle
        auto filtered = buildFiltered(state);
        profNames->clear();
        for (auto& [idx, p] : filtered) profNames->push_back(p->name);
        if (profNames->empty()) {
            return vbox({
                headerBar("Profession"),
                text("  No professions available.") | color(kError),
            }) | border;
        }
        *profMenu = std::min(*profMenu, (int)profNames->size() - 1);
        const auto& prof  = *filtered[*profMenu].second;
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
            bpBar(state.bpRemaining(), state.bpTotal()),
        }) | border;
    });

    return CatchEvent(renderer, [&app, profMenu, menu, statusMsg, profNames, buildFiltered](Event e) -> bool {
        auto& state = app.state();
        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::Return) {
            auto filtered = buildFiltered(state);
            if (!filtered.empty() && *profMenu < (int)filtered.size()) {
                state.character.profession = filtered[*profMenu].second->name;
                state.markComplete(Section::Profession);
            }
            *statusMsg = "";
            app.navigate(Section::Dashboard);
            return true;
        }
        return menu->OnEvent(e);
    });
}

} // namespace dod
