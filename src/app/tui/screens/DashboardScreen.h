#pragma once
#include "../TuiState.h"
#include "../TuiApp.h"
#include <ftxui/component/component.hpp>

namespace dod {

/// Factory: returns the Dashboard component.
/// The dashboard is the main navigation hub — arrow keys cycle sections,
/// Enter navigates into the selected section.
ftxui::Component MakeDashboardScreen(TuiApp& app);

} // namespace dod
