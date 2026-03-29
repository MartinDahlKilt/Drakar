#include "AppearanceScreen.h"
#include "../TuiTheme.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

namespace dod {

ftxui::Component MakeAppearanceScreen(TuiApp& app) {
    using namespace theme;

    auto appearance  = std::make_shared<std::string>(app.state().character.appearance);
    auto personality = std::make_shared<std::string>(app.state().character.personality);
    auto background  = std::make_shared<std::string>(app.state().character.background);

    InputOption opt;
    opt.multiline = false;
    auto input1 = Input(appearance.get(),  opt);
    auto input2 = Input(personality.get(), opt);
    auto input3 = Input(background.get(),  opt);

    auto layout = Container::Vertical({input1, input2, input3});

    auto renderer = Renderer(layout, [&app, appearance, personality, background, input1, input2, input3, layout]() -> Element {
        return vbox({
            headerBar("Appearance & Background"),
            vbox({
                text("  All fields are optional. Press Tab to move between fields.") | color(kDim),
                text("  Press D or Enter when done.") | color(kDim),
                text(""),
                text("Physical appearance") | bold | color(kHeader),
                text("  (height, build, hair, eyes, distinguishing marks)") | color(kDim),
                input1->Render() | border,
                text(""),
                text("Personality & mannerisms") | bold | color(kHeader),
                input2->Render() | border,
                text(""),
                text("Background story") | bold | color(kHeader),
                text("  (origin, family, reason for adventuring)") | color(kDim),
                input3->Render() | border,
                filler(),
                text(""),
                keyBar({{"Tab","Next field"}, {"Esc","Done / Back"}}),
            }) | flex ,
        }) | border;
    });

    return CatchEvent(renderer, [&app, layout, appearance, personality, background](Event e) -> bool {
        if (e == Event::Escape) {
            auto& state = app.state();
            state.character.appearance  = *appearance;
            state.character.personality = *personality;
            state.character.background  = *background;
            state.markComplete(Section::Appearance);
            app.navigate(Section::Dashboard);
            return true;
        }
        return layout->OnEvent(e);
    });
}

} // namespace dod
