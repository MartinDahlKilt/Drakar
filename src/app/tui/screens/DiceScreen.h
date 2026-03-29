#pragma once
#include "../TuiState.h"
#include "../TuiApp.h"
#include <ftxui/component/component.hpp>
#include <string>

namespace dod {

enum class DiceContext {
    SpecialAbility,
    WeaponHand,
    Social,
    Capital,
};

ftxui::Component MakeDiceScreen(TuiApp& app, DiceContext ctx);

} // namespace dod
