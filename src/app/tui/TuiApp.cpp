#include "TuiApp.h"
#include "screens/DashboardScreen.h"
#include "screens/NameScreen.h"
#include "screens/ProfessionScreen.h"
#include "screens/RaceScreen.h"
#include "screens/StatsScreen.h"
#include "screens/StoScreen.h"
#include "screens/DiceScreen.h"
#include "screens/AgeScreen.h"
#include "screens/SkillsScreen.h"
#include "screens/AppearanceScreen.h"
#include "screens/SummaryScreen.h"
#include "screens/SettingsScreen.h"
#include "screens/BPLevelScreen.h"
#include "screens/SynHorselScreen.h"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;
using json = nlohmann::json;
namespace fs = std::filesystem;

namespace dod {

TuiApp::TuiApp(DiceRoller& dice, const char* argv0)
    : screen_(ScreenInteractive::Fullscreen())
    , dice_(dice)
{
    // Resolve config file path: same directory as the executable
    if (argv0) {
        try {
            fs::path exePath = fs::canonical(fs::path(argv0));
            configPath_ = (exePath.parent_path() / "dod_config.json").string();
        } catch (...) {
            configPath_ = "dod_config.json";
        }
    } else {
        configPath_ = "dod_config.json";
    }

    loadConfig();
    switchTo(Section::Dashboard);
}

void TuiApp::run() {
    // Wrap activeComponent_ in an indirection so we can swap it live.
    // We copy activeComponent_ into a local shared_ptr before each call so
    // that navigate() (called from inside OnEvent/Render) can safely replace
    // activeComponent_ without destroying the component while it is still on
    // the call stack.
    auto root = CatchEvent(
        Renderer([this]() -> Element {
            auto current = activeComponent_;
            return current ? current->Render() : text("Starting…");
        }),
        [this](Event e) -> bool {
            if (e == Event::Special("\x03")) {  // Ctrl+C
                quit();
                return true;
            }
            auto current = activeComponent_;
            return current ? current->OnEvent(e) : false;
        }
    );
    screen_.Loop(root);
}

void TuiApp::navigate(Section s) {
    // Remember which non-Dashboard screen we're leaving so the Dashboard
    // can restore the cursor to that row.
    if (s == Section::Dashboard && state_.currentSection != Section::Dashboard) {
        state_.lastSection = state_.currentSection;
    }
    state_.currentSection = s;
    switchTo(s);
    screen_.PostEvent(Event::Custom);
}

void TuiApp::quit() {
    screen_.ExitLoopClosure()();
}

void TuiApp::loadConfig() {
    if (configPath_.empty()) return;
    std::ifstream f(configPath_);
    if (!f.is_open()) return;
    try {
        json j = json::parse(f);
        if (j.contains("allow_anka") && j["allow_anka"].is_boolean())
            state_.allowAnka = j["allow_anka"].get<bool>();
        if (j.contains("allow_warrior_expansion") && j["allow_warrior_expansion"].is_boolean())
            state_.allowWarriorExpansion = j["allow_warrior_expansion"].get<bool>();
    } catch (...) {
        // Corrupt or unreadable config — silently ignore
    }
}

void TuiApp::saveConfig() {
    if (configPath_.empty()) return;
    try {
        json j;
        j["allow_anka"]               = state_.allowAnka;
        j["allow_warrior_expansion"]  = state_.allowWarriorExpansion;
        std::ofstream f(configPath_);
        if (f.is_open())
            f << j.dump(2) << "\n";
    } catch (...) {
        // Non-fatal: ignore write failures
    }
}

void TuiApp::switchTo(Section s) {
    switch (s) {
        case Section::Dashboard:      activeComponent_ = MakeDashboardScreen(*this);  break;
        case Section::Name:           activeComponent_ = MakeNameScreen(*this);        break;
        case Section::Profession:     activeComponent_ = MakeProfessionScreen(*this);  break;
        case Section::Race:           activeComponent_ = MakeRaceScreen(*this);        break;
        case Section::Stats:          activeComponent_ = MakeStatsScreen(*this);       break;
        case Section::STO:            activeComponent_ = MakeStoScreen(*this);         break;
        case Section::SpecialAbility: activeComponent_ = MakeDiceScreen(*this, DiceContext::SpecialAbility); break;
        case Section::WeaponHand:     activeComponent_ = MakeDiceScreen(*this, DiceContext::WeaponHand);     break;
        case Section::Social:         activeComponent_ = MakeDiceScreen(*this, DiceContext::Social);         break;
        case Section::Capital:        activeComponent_ = MakeDiceScreen(*this, DiceContext::Capital);        break;
        case Section::Age:            activeComponent_ = MakeAgeScreen(*this);         break;
        case Section::Skills:         activeComponent_ = MakeSkillsScreen(*this);      break;
        case Section::Appearance:     activeComponent_ = MakeAppearanceScreen(*this);  break;
        case Section::Summary:        activeComponent_ = MakeSummaryScreen(*this);     break;
        case Section::Settings:       activeComponent_ = MakeSettingsScreen(*this);    break;
        case Section::BPLevel:         activeComponent_ = MakeBPLevelScreen(*this);     break;
        case Section::SynHorsel:       activeComponent_ = MakeSynHorselScreen(*this);   break;
        default:                      activeComponent_ = MakeDashboardScreen(*this);   break;
    }
}

} // namespace dod
