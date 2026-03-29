#pragma once
#include "TuiState.h"
#include "core/Dice.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <memory>

namespace dod {

/// Top-level application class. Owns the ScreenInteractive, TuiState, DiceRoller,
/// and all screen components. Call run() to enter the interactive loop.
class TuiApp {
public:
    explicit TuiApp(DiceRoller& dice);

    /// Blocking: runs the ScreenInteractive loop until the user quits.
    void run();

    /// Navigate to a section. Safe to call from within component event handlers.
    void navigate(Section s);

    /// Request exit from the event loop.
    void quit();

    TuiState&   state()       { return state_; }
    DiceRoller& dice()        { return dice_; }
    ftxui::ScreenInteractive& screen() { return screen_; }

private:
    ftxui::ScreenInteractive screen_;
    DiceRoller&              dice_;
    TuiState                 state_;

    /// The single Component rendered by the event loop.
    /// Swapped when navigate() is called.
    ftxui::Component         activeComponent_;

    /// Build the component for the given section and set it as active.
    void switchTo(Section s);
};

} // namespace dod
