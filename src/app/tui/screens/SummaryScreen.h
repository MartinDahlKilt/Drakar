#pragma once
#include "../TuiState.h"
#include "../TuiApp.h"
#include <ftxui/component/component.hpp>

namespace dod {

ftxui::Component MakeSummaryScreen(TuiApp& app);

} // namespace dod
