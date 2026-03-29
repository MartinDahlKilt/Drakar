#pragma once
#include "TuiState.h"
#include "core/Dice.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <memory>
#include <string>

namespace dod {

/// Top-level application class. Owns the ScreenInteractive, TuiState, DiceRoller,
/// and all screen components. Call run() to enter the interactive loop.
class TuiApp {
public:
    /// @param dice     Dice roller instance.
    /// @param argv0    Value of argv[0] — used to locate the config file.
    explicit TuiApp(DiceRoller& dice, const char* argv0 = nullptr);

    /// Blocking: runs the ScreenInteractive loop until the user quits.
    void run();

    /// Navigate to a section. Safe to call from within component event handlers.
    void navigate(Section s);

    /// Request exit from the event loop.
    void quit();

    /// Persist current settings to the config file.
    void saveConfig();

    TuiState&   state()       { return state_; }
    DiceRoller& dice()        { return dice_; }
    ftxui::ScreenInteractive& screen() { return screen_; }

private:
    ftxui::ScreenInteractive screen_;
    DiceRoller&              dice_;
    TuiState                 state_;
    std::string              configPath_;

    /// The single Component rendered by the event loop.
    /// Swapped when navigate() is called.
    ftxui::Component         activeComponent_;

    /// Build the component for the given section and set it as active.
    void switchTo(Section s);

    /// Load settings from configPath_ into state_ (silently ignores missing file).
    void loadConfig();
};

} // namespace dod
