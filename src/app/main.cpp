#include "tui/TuiApp.h"
#include "core/Dice.h"
#include <iostream>
#include <stdexcept>

int main(int /*argc*/, char* argv[]) {
    try {
        dod::DiceRoller dice;
        dod::TuiApp     app(dice, argv[0]);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
