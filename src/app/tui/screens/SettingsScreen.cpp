#include "SettingsScreen.h"
#include "../TuiTheme.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <functional>
#include <string>
#include <vector>

using namespace ftxui;

namespace dod {

ftxui::Component MakeSettingsScreen(TuiApp& app) {
    using namespace theme;

    struct SettingDef {
        std::string label;
        std::string desc;
        std::function<bool()>       get;
        std::function<void(bool)>   set;
    };

    // All settings are listed here. Add new entries to extend the screen.
    auto settings = std::make_shared<std::vector<SettingDef>>();
    settings->push_back({
        "Anka race",
        "Allow the Anka (Duck) race to appear in the race selection list.",
        [&app]() { return app.state().allowAnka; },
        [&app](bool v) {
            app.state().allowAnka = v;
            if (!v && app.state().character.race == "Anka")
                app.state().refundRace();
        },
    });

    auto selected = std::make_shared<int>(0);

    auto toggle = [settings, selected]() {
        auto& def = (*settings)[*selected];
        def.set(!def.get());
    };

    auto renderer = Renderer([&app, settings, selected]() -> Element {
        int n = (int)settings->size();
        Elements rows;
        for (int i = 0; i < n; ++i) {
            const auto& def = (*settings)[i];
            bool val = def.get();
            bool isSel = (i == *selected);

            Element row = hbox({
                text("  "),
                text(def.label) | bold | color(kValue) | size(WIDTH, EQUAL, 22),
                text(val ? " [ON] " : " [OFF]") | bold | color(val ? kSuccess : kDim),
                text("  "),
                text(def.desc) | color(kDim),
            });

            if (isSel)
                row = row | bgcolor(Color::Blue) | color(Color::White);

            rows.push_back(row);
            rows.push_back(text(""));
        }

        return vbox({
            theme::headerBar("Settings"),
            vbox({
                text(""),
                text("  Rules & Options") | bold | color(kHeader),
                separator() | color(kDim),
                text(""),
                vbox(rows),
                filler(),
                separator() | color(kDim),
                keyBar({{"↑↓","Select"}, {"Space/Enter","Toggle"}, {"Esc","Back"}}),
            }) | flex,
        }) | border;
    });

    return CatchEvent(renderer, [&app, settings, selected, toggle](Event e) -> bool {
        int n = (int)settings->size();
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
        if (e == Event::Character(' ') || e == Event::Return) {
            toggle();
            return true;
        }
        return false;
    });
}

} // namespace dod

