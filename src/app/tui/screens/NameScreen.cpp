#include "NameScreen.h"
#include "../TuiTheme.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

namespace dod {

ftxui::Component MakeNameScreen(TuiApp& app) {
    using namespace theme;

    auto  name  = std::make_shared<std::string>(app.state().character.name);
    auto  genderIdx = std::make_shared<int>(app.state().character.gender == "Female" ? 1 : 0);
    auto  errorMsg  = std::make_shared<std::string>("");

    InputOption nameOpt;
    nameOpt.placeholder = "Enter character name…";
    auto nameInput = Input(name.get(), nameOpt);

    static const std::vector<std::string> genderOpts = {"Male", "Female"};
    auto genderMenu = Menu(&genderOpts, genderIdx.get());

    auto confirm = [&app, name, genderIdx, errorMsg]() -> bool {
        if (name->empty()) {
            *errorMsg = "Name cannot be empty.";
            return false;
        }
        auto& state = app.state();
        state.character.name   = *name;
        state.character.gender = (*genderIdx == 0) ? "Male" : "Female";
        state.markComplete(Section::Name);
        *errorMsg = "";
        app.navigate(Section::Dashboard);
        return true;
    };

    auto layout = Container::Vertical({
        nameInput,
        genderMenu,
    });

    auto renderer = Renderer(layout, [&app, name, genderIdx, errorMsg, nameInput, genderMenu, layout]() -> Element {
        Element body = vbox({
            headerBar("Name & Gender"),
            vbox({
                text("Character Name") | bold | color(kHeader),
                nameInput->Render() | border,
                text(""),
                text("Gender") | bold | color(kHeader),
                genderMenu->Render() | border,
                text(""),
                errorMsg->empty()
                    ? text("")
                    : text("  ✗ " + *errorMsg) | color(kError),
                text(""),
                keyBar({{"Enter","Confirm"}, {"Esc","Back to Dashboard"}}),
            }) | flex ,
        }) | border;
        return body;
    });

    return CatchEvent(renderer, [&app, layout, confirm, name, genderIdx](Event e) -> bool {
        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::Return) {
            // If focus is on the name input, move to gender; if on gender, confirm.
            if (!confirm()) return true;
            return true;
        }
        return layout->OnEvent(e);
    });
}

} // namespace dod
