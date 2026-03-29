#include "DashboardScreen.h"
#include "../TuiTheme.h"

#include "core/GameRules.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

namespace dod {

ftxui::Component MakeDashboardScreen(TuiApp& app) {
    using namespace theme;

    struct Row {
        Section    section;
        std::string label;
    };

    static const std::vector<Row> kRows = {
        { Section::Profession,     "Profession"      },
        { Section::Race,           "Race"            },
        { Section::Stats,          "Statistics"      },
        { Section::STO,            "Size (STO)"      },
        { Section::SpecialAbility, "Special Ability" },
        { Section::WeaponHand,     "Weapon Hand"     },
        { Section::Social,         "Social Standing" },
        { Section::Capital,        "Starting Capital"},
        { Section::Age,            "Age Category"    },
        { Section::Skills,         "Skills"          },
        { Section::Name,           "Name & Gender"   },
        { Section::Appearance,     "Appearance"      },
        { Section::Summary,        "Summary & Save"  },
        { Section::Settings,       "Settings"        },
    };

    auto selected  = std::make_shared<int>(1); // default highlight on Profession
    auto blockMsg  = std::make_shared<std::string>("");
    // If returning from a screen, place the cursor on that screen's row
    {
        const Section current = app.state().lastSection;
        for (int i = 0; i < (int)kRows.size(); ++i) {
            if (kRows[i].section == current) {
                *selected = i;
                break;
            }
        }
    }

    auto renderer = Renderer([&app, selected, blockMsg]() -> Element {
        auto& state = app.state();
        int numRows = (int)kRows.size();

        // ---- Build section overview with current values ----
        auto getSectionValue = [&state](Section s) -> std::string {
            switch (s) {
                case Section::Name:
                    return state.character.name.empty()
                        ? "—"
                        : state.character.name + "  (" + state.character.gender + ")";
                case Section::Profession:
                    return state.character.profession.empty() ? "—" : state.character.profession;
                case Section::Race:
                    return state.character.race.empty() ? "—" : state.character.race;
                case Section::Stats: {
                    if (!state.isComplete(Section::Stats)) return "—";
                    auto f = state.computeFinalStats();
                    return "STY" + std::to_string(f.STY) + " FYS" + std::to_string(f.FYS)
                         + " SMI" + std::to_string(f.SMI) + " INT" + std::to_string(f.INT)
                         + " PSY" + std::to_string(f.PSY) + " KAR" + std::to_string(f.KAR);
                }
                case Section::STO:
                    return state.isComplete(Section::STO)
                        ? "STO " + std::to_string(state.character.purchasedStats.STO)
                        : "—";
                case Section::SpecialAbility:
                    return state.character.hasSpecialAbility
                        ? state.character.specialAbilityDesc.substr(0, 30) + "…"
                        : state.isComplete(Section::SpecialAbility) ? "Skipped" : "—";
                case Section::WeaponHand:
                    if (!state.character.weaponHand.empty())
                        return state.character.weaponHandFromSpecialAbility
                               ? state.character.weaponHand + " (from ability)"
                               : state.character.weaponHand;
                    if (!state.isComplete(Section::SpecialAbility))
                        return "needs Special Ability first";
                    return "—";
                case Section::Social:
                    return state.character.socialDescription.empty()
                        ? "—" : state.character.socialDescription;
                case Section::Capital:
                    if (state.isComplete(Section::Capital))
                        return std::to_string(state.character.startingCapital) + " sm";
                    if (!state.isComplete(Section::Social))
                        return "needs Social Standing first";
                    return "—";
                case Section::Age:
                    if (!state.character.ageCategory.empty())
                        return state.character.ageCategory;
                    if (!state.isComplete(Section::Capital))
                        return "needs Starting Capital first";
                    return "—";
                case Section::Skills:
                    return state.isComplete(Section::Skills)
                        ? std::to_string(state.character.skills.size()) + " skills"
                        : "—";
                case Section::Appearance:
                    return state.isComplete(Section::Appearance)
                        ? (state.character.appearance.empty() ? "Skipped" : state.character.appearance.substr(0,25)+"…")
                        : "—";
                case Section::Summary:
                    return state.isReadyToSave() ? "Ready to save" : "Not ready";
                case Section::Settings:
                    return state.allowAnka ? "Anka: ON" : "Anka: OFF";
                default: return "";
            }
        };

        // ---- Left panel: section menu ----
        Elements menuRows;
        for (int i = 0; i < numRows; ++i) {
            const auto& row = kRows[i];
            bool isSelected = (i == *selected);
            bool complete   = state.isComplete(row.section);
            bool visited    = state.currentSection == row.section;

            auto indicator = sectionStatus(complete, visited);
            bool weaponHandBlocked = (row.section == Section::WeaponHand
                                      && !state.isComplete(Section::SpecialAbility));
            bool capitalBlocked    = (row.section == Section::Capital
                                      && !state.isComplete(Section::Social));
            bool ageBlocked        = (row.section == Section::Age
                                      && !state.isComplete(Section::Capital));
            bool isBlocked = weaponHandBlocked || capitalBlocked || ageBlocked;
            Element labelEl = isBlocked
                ? hbox({text(" " + row.label), text(" 🔒") | color(kDim)})
                : row.section == Section::Settings
                    ? hbox({text("⚙  " + row.label + " ")})
                    : text(" " + row.label + " ");
            Color valueColor = isBlocked ? kError : kDim;
            auto valueEl   = text(getSectionValue(row.section)) | color(valueColor);

            Element rowEl = hbox({
                indicator,
                text(" "),
                labelEl,
                filler(),
                valueEl,
            });

            if (isSelected) {
                rowEl = rowEl | bgcolor(Color::Blue) | color(Color::White) | bold;
            }
            menuRows.push_back(rowEl);
        }

        // ---- Right panel: live character summary ----
        const auto& c  = state.character;
        const auto  fs = state.computeFinalStats();

        auto row = [](const std::string& lbl, const std::string& val, Color vc = theme::kValue) {
            return hbox({
                text(" " + lbl) | color(theme::kDim) | size(WIDTH, EQUAL, 14),
                text(val) | color(vc),
            });
        };

        // Stats line
        std::string statsStr = "—";
        if (state.isComplete(Section::Stats)) {
            statsStr = "STY" + std::to_string(fs.STY)
                     + " FYS" + std::to_string(fs.FYS)
                     + " SMI" + std::to_string(fs.SMI)
                     + " INT" + std::to_string(fs.INT)
                     + " PSY" + std::to_string(fs.PSY)
                     + " KAR" + std::to_string(fs.KAR)
                     + " STO" + std::to_string(fs.STO);
        }

        // Skills summary
        int profSkillCount = 0;
        for (const auto& sk : c.skills)
            if (sk.isProfessionSkill) ++profSkillCount;

        auto rightPanel = vbox({
            text(" Character") | bold | color(theme::kHeader),
            separator() | color(theme::kDim),
            row("Profession:", c.profession.empty()   ? "—" : c.profession),
            row("Race:",       c.race.empty()          ? "—" : c.race),
            row("Age:",        c.ageCategory.empty()   ? "—" : c.ageCategory),
            row("Name:",       c.name.empty()          ? "—" : c.name + " (" + c.gender + ")"),
            text(""),
            text(" Statistics") | bold | color(theme::kHeader),
            separator() | color(theme::kDim),
            text(" " + statsStr) | color(theme::kValue),
            text(""),
            text(" Derived") | bold | color(theme::kHeader),
            separator() | color(theme::kDim),
            row("HP:",         state.isComplete(Section::Stats) ? std::to_string(GameRules::calculateHP(fs.FYS, fs.STO)) : "—"),
            row("Dmg bonus:",  state.isComplete(Section::Stats) ? GameRules::calculateDamageBonus(fs.STY, fs.STO) : "—"),
            row("Movement:",   state.isComplete(Section::Stats) ? std::to_string(GameRules::calculateMovement(fs.STO, fs.FYS, fs.SMI, c.race)) + " sq" : "—"),
            text(""),
            text(" Dice rolls") | bold | color(theme::kHeader),
            separator() | color(theme::kDim),
            row("Spc. ability:", !c.specialAbilityDesc.empty() ? c.specialAbilityDesc.substr(0, 22) + (c.specialAbilityDesc.size() > 22 ? "…" : "")
                                 : state.isComplete(Section::SpecialAbility) ? "Skipped" : "—"),
            row("Weapon hand:", c.weaponHand.empty()           ? "—" : c.weaponHand),
            row("Social:",      c.socialDescription.empty()    ? "—" : c.socialDescription),
            row("Capital:",     state.isComplete(Section::Capital) ? std::to_string(c.startingCapital) + " sm" : "—"),
            text(""),
            text(" Skills") | bold | color(theme::kHeader),
            separator() | color(theme::kDim),
            row("Profession:", profSkillCount > 0 ? std::to_string(profSkillCount) + " chosen" : "—"),
            text(""),
            text(" Experience") | bold | color(theme::kHeader),
            separator() | color(theme::kDim),
            row("EP (age):",  state.isComplete(Section::Age) ? std::to_string(c.epFromAge) : "—"),
            row("EP (BP):",   std::to_string(state.bpRemaining()) + " × 5 = " + std::to_string(state.bpRemaining() * 5)),
            row("Total EP:",  state.isComplete(Section::Age)
                              ? std::to_string(c.epFromAge + state.bpRemaining() * 5) : "—",
                              theme::kSuccess),
            filler(),
            separator() | color(theme::kDim),
            bpBar(state.bpRemaining(), 125),
            text(""),
            text(state.isReadyToSave() ? "  ✓ Ready to save!" : "  Complete required sections to save.")
                | color(state.isReadyToSave() ? theme::kSuccess : theme::kDim),            // Warn if any final stat is zero or below after age selection
            [&]() -> Element {
                if (!state.isComplete(Section::Age)) return text("");
                const auto fs2 = state.computeFinalStats();
                const char* sn[] = {"STY","FYS","SMI","INT","PSY","KAR","STO"};
                const int   sv[] = {fs2.STY,fs2.FYS,fs2.SMI,fs2.INT,fs2.PSY,fs2.KAR,fs2.STO};
                std::string bad;
                for (int i = 0; i < 7; ++i) {
                    if (sv[i] <= 0) {
                        if (!bad.empty()) bad += ", ";
                        bad += std::string(sn[i]) + "=" + std::to_string(sv[i]);
                    }
                }
                if (bad.empty()) return text("");
                return text("  \u26a0 Stats at zero: " + bad + " \u2014 raise stats to save.")
                       | color(theme::kError) | bold;
            }(),            blockMsg->empty() ? text("") : text("  ⚠ " + *blockMsg) | color(theme::kError) | bold,
        }) | flex;

        return vbox({
            headerBar("Dashboard"),
            hbox({
                vbox(menuRows) | size(WIDTH, EQUAL, 50),
                separator() | color(kDim),
                rightPanel ,
            }) | flex,
        }) | border;
    });

    auto eventHandler = CatchEvent(renderer, [&app, selected, blockMsg](Event event) -> bool {
        int numRows = (int)kRows.size();
        if (event == Event::ArrowUp) {
            *selected = (*selected - 1 + numRows) % numRows;
            return true;
        }
        if (event == Event::ArrowDown) {
            *selected = (*selected + 1) % numRows;
            return true;
        }
        if (event == Event::Return) {
            Section target = kRows[*selected].section;
            auto& st = app.state();
            if (target == Section::WeaponHand && !st.isComplete(Section::SpecialAbility)) {
                *blockMsg = "Roll Special Ability before Weapon Hand.";
                return true;
            }
            if (target == Section::Capital && !st.isComplete(Section::Social)) {
                *blockMsg = "Roll Social Standing before Starting Capital.";
                return true;
            }
            if (target == Section::Age && !st.isComplete(Section::Capital)) {
                *blockMsg = "Roll Starting Capital before Age Category.";
                return true;
            }
            *blockMsg = "";
            app.navigate(target);
            return true;
        }
        if (event == Event::Character('q') || event == Event::Character('Q')) {
            app.quit();
            return true;
        }
        return false;
    });

    return eventHandler;
}

} // namespace dod
