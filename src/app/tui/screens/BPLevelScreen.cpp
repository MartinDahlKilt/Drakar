#include "BPLevelScreen.h"
#include "../TuiTheme.h"

#include "core/GameData.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

namespace dod {

ftxui::Component MakeBPLevelScreen(TuiApp& app) {
    using namespace theme;

    auto selected = std::make_shared<int>(app.state().bpLevelIndex);

    auto renderer = Renderer([&app, selected]() -> Element {
        const auto& levels = GameData::getBPLevels();
        Elements rows;

        for (int i = 0; i < (int)levels.size(); ++i) {
            const auto& lvl = levels[i];
            bool isSel = (i == *selected);

            std::string epStr = "EP: " + std::to_string(lvl.ep[0]) + "/"
                              + std::to_string(lvl.ep[1]) + "/"
                              + std::to_string(lvl.ep[2]) + "/"
                              + std::to_string(lvl.ep[3]);
            std::string fvStr = "MaxFV: " + std::to_string(lvl.maxFV[0]) + "/"
                              + std::to_string(lvl.maxFV[1]) + "/"
                              + std::to_string(lvl.maxFV[2]) + "/"
                              + std::to_string(lvl.maxFV[3]);
            std::string saStr = "SA rolls: " + std::to_string(lvl.specialAbilityRolls);
            std::string bpStr = std::to_string(lvl.bp) + " BP";

            Element row = hbox({
                text("  "),
                text(lvl.name) | bold | size(WIDTH, EQUAL, 16),
                text(bpStr)    | color(kAccent) | size(WIDTH, EQUAL, 10),
                text(saStr)    | color(kValue)  | size(WIDTH, EQUAL, 14),
                text(epStr)    | color(kDim)    | size(WIDTH, EQUAL, 30),
                text(fvStr)    | color(kDim),
            });

            if (isSel)
                row = row | bgcolor(Color::Blue) | color(Color::White) | bold;

            rows.push_back(row);
            rows.push_back(text(""));
        }

        // Column headers
        Element header = hbox({
            text("  "),
            text("Level")        | bold | color(kHeader) | size(WIDTH, EQUAL, 16),
            text("BP")           | bold | color(kHeader) | size(WIDTH, EQUAL, 10),
            text("SA Rolls")     | bold | color(kHeader) | size(WIDTH, EQUAL, 14),
            text("EP (Y/M/M/O)") | bold | color(kHeader) | size(WIDTH, EQUAL, 30),
            text("MaxFV (Y/M/M/O)") | bold | color(kHeader),
        });

        std::string current = GameData::getBPLevels()[*selected].name;

        return vbox({
            headerBar("BP Level — Krigarexpansion"),
            vbox({
                text(""),
                text("  Choose your character power level.") | color(kDim),
                text("  Y=Young  M=Mature  M=Middle-aged  O=Old") | color(kDim),
                text(""),
                header,
                separator() | color(kDim),
                text(""),
                vbox(rows),
                filler(),
                separator() | color(kDim),
                hbox({
                    text("  Selected: ") | color(kDim),
                    text(current) | bold | color(kValue),
                }),
                text(""),
                keyBar({{"↑↓","Select level"}, {"Enter","Confirm"}, {"Esc","Back"}}),
            }) | flex,
        }) | border;
    });

    return CatchEvent(renderer, [&app, selected](Event e) -> bool {
        const int n = (int)GameData::getBPLevels().size();
        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::ArrowUp) {
            *selected = (*selected - 1 + n) % n;
            return true;
        }
        if (e == Event::ArrowDown) {
            *selected = (*selected + 1) % n;
            return true;
        }
        if (e == Event::Return) {
            auto& state = app.state();
            state.bpLevelIndex                 = *selected;
            state.character.bpLevelIndex       = *selected;
            state.character.useWarriorExpansion = true;
            state.markComplete(Section::BPLevel);
            app.navigate(Section::Dashboard);
            return true;
        }
        return false;
    });
}

} // namespace dod
