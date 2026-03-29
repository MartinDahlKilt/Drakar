#include "SummaryScreen.h"
#include "../TuiTheme.h"

#include "core/CharacterSerializer.h"
#include "core/GameData.h"
#include "core/GameRules.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <algorithm>
#include <sstream>

using namespace ftxui;

namespace dod {

ftxui::Component MakeSummaryScreen(TuiApp& app) {
    using namespace theme;

    // Prepare default filename
    auto filename = std::make_shared<std::string>(app.state().character.name);
    std::replace(filename->begin(), filename->end(), ' ', '_');
    if (!filename->empty()) *filename += ".json";
    else *filename = "character.json";

    auto saveMsg    = std::make_shared<std::string>("");
    auto saveMsgOk  = std::make_shared<bool>(false);

    // Characters that are invalid in filenames (cross-platform: Linux + Windows)
    static constexpr std::string_view kInvalidChars = "/\\:*?\"<>|";

    InputOption fnOpt;
    fnOpt.placeholder = "character.json";
    fnOpt.on_change = [filename]() {
        std::string filtered;
        filtered.reserve(filename->size());
        for (unsigned char ch : *filename) {
            if (ch < 0x20) continue;  // strip control characters
            if (kInvalidChars.find(static_cast<char>(ch)) != std::string_view::npos) continue;
            filtered += static_cast<char>(ch);
        }
        if (filtered != *filename)
            *filename = std::move(filtered);
    };
    auto filenameInput = Input(filename.get(), fnOpt);

    auto layout = Container::Vertical({filenameInput});

    auto doSave = [&app, filename, saveMsg, saveMsgOk]() {
        auto& state = app.state();
        std::string path = *filename;
        if (path.empty()) path = "character.json";
        if (path.size() < 5 || path.substr(path.size()-5) != ".json") path += ".json";

        // Finalize EP (in case age wasn't re-visited after spending BP)
        state.character.epFromBP  = state.bpRemaining() * 5;
        state.character.epTotal   = state.character.epFromAge + state.character.epFromBP;
        state.character.bpSpent     = state.bpBreakdown.total();
        state.character.bpRemaining = state.bpRemaining();

        try {
            CharacterSerializer::saveToFile(state.character, path);
            *saveMsg   = "Character saved to: " + path;
            *saveMsgOk = true;
            state.markComplete(Section::Summary);
        } catch (const std::exception& ex) {
            *saveMsg   = std::string("Error: ") + ex.what();
            *saveMsgOk = false;
        }
    };

    auto renderer = Renderer(layout, [&app, filename, saveMsg, saveMsgOk, filenameInput, layout]() -> Element {
        auto& state = app.state();
        const auto& c = state.character;
        auto f = state.computeFinalStats();

        // Stats table
        auto statRow = [&](const std::string& name, int val, int req) -> Element {
            bool ok = (req == 0 || val >= req);
            return hbox({
                text("  " + name + ": ") | color(kDim),
                text(std::to_string(val)) | bold | color(ok ? kValue : kError),
                req > 0 ? text(" (req ≥" + std::to_string(req)+")") | color(ok ? kSuccess : kError)
                        : text(""),
            });
        };

        int reqSTY=0, reqFYS=0, reqSMI=0, reqINT=0, reqPSY=0, reqKAR=0;
        if (!c.profession.empty()) {
            auto profOpt = GameData::findProfession(c.profession);
            if (profOpt) {
                auto getR = [&](const std::string& s) -> int {
                    auto it = profOpt->statRequirements.find(s);
                    return it != profOpt->statRequirements.end() ? it->second : 0;
                };
                reqSTY=getR("STY"); reqFYS=getR("FYS"); reqSMI=getR("SMI");
                reqINT=getR("INT"); reqPSY=getR("PSY"); reqKAR=getR("KAR");
            }
        }

        // Skill list
        Elements skillEls;
        for (const auto& sk : c.skills) {
            skillEls.push_back(hbox({
                text(sk.isPrimary ? "  ○ " : "  ★ ") | color(sk.isPrimary ? kDim : kAccent),
                text(sk.name) | color(kValue),
                text("  FV=" + std::to_string(sk.fv)) | color(kBPColor),
                text("  (" + sk.baseStat + ")") | color(kDim),
            }));
        }

        Element leftPanel = vbox({
            text("Identity") | bold | color(kHeader),
            separator() | color(kDim),
            labelValue("Name:       ", c.name.empty() ? "—" : c.name),
            labelValue("Gender:     ", c.gender),
            labelValue("Race:       ", c.race.empty() ? "—" : c.race),
            labelValue("Profession: ", c.profession.empty() ? "—" : c.profession),
            labelValue("Age:        ", c.ageCategory.empty() ? "—" : c.ageCategory),
            text(""),
            text("Statistics") | bold | color(kHeader),
            separator() | color(kDim),
            statRow("STY", f.STY, reqSTY),
            statRow("FYS", f.FYS, reqFYS),
            statRow("SMI", f.SMI, reqSMI),
            statRow("INT", f.INT, reqINT),
            statRow("PSY", f.PSY, reqPSY),
            statRow("KAR", f.KAR, reqKAR),
            statRow("STO", f.STO, 0),
            text(""),
            text("Derived") | bold | color(kHeader),
            separator() | color(kDim),
            labelValue("HP total:       ", std::to_string(c.derived.hpTotal)),
            labelValue("Damage bonus:   ", c.derived.damageBonus.empty() ? "—" : c.derived.damageBonus),
            labelValue("Movement:       ", std::to_string(c.derived.movement) + " sq/round"),
            text(""),
            text("Economic") | bold | color(kHeader),
            separator() | color(kDim),
            labelValue("Social standing: ", c.socialDescription.empty() ? "—" : c.socialDescription),
            labelValue("Capital:         ", std::to_string(c.startingCapital) + " sm"),
            labelValue("Weapon hand:     ", c.weaponHand.empty() ? "—" : c.weaponHand),
        });

        // Compute EP live so it always reflects current unspent BP
        const int liveEpFromBP = state.bpRemaining() * 5;
        const int liveEpTotal  = c.epFromAge + liveEpFromBP;

        Element rightPanel = vbox({
            text("Experience") | bold | color(kHeader),
            separator() | color(kDim),
            labelValue("EP from age:  ", std::to_string(c.epFromAge)),
            labelValue("EP from BP:   ", std::to_string(liveEpFromBP)),
            labelValue("Total EP:     ", std::to_string(liveEpTotal), kSuccess),
            text(""),
            text("Background Points") | bold | color(kHeader),
            separator() | color(kDim),
            labelValue("Total:      ", std::to_string(state.bpTotal())),
            labelValue("Spent:      ", std::to_string(state.bpBreakdown.total())),
            labelValue("Remaining:  ", std::to_string(state.bpRemaining()), kBPColor),
            text(""),
            // Warrior expansion block
            [&]() -> Element {
                if (!state.allowWarriorExpansion) return text("");
                auto& levels = GameData::getBPLevels();
                std::string lvlName = (state.bpLevelIndex < (int)levels.size())
                    ? levels[state.bpLevelIndex].name : "—";
                int sp = state.isComplete(Section::Stats) ? GameRules::calculateSP(f.FYS) : 0;
                std::string synStr  = std::to_string(c.synRoll)   + " ("
                    + (c.synMod >= 0 ? "+" : "") + std::to_string(c.synMod) + ")";
                std::string horStr  = std::to_string(c.horselRoll) + " ("
                    + (c.horselMod >= 0 ? "+" : "") + std::to_string(c.horselMod) + ")";
                return vbox({
                    text("Warrior Expansion") | bold | color(kHeader),
                    separator() | color(kDim),
                    labelValue("BP Level:  ", lvlName),
                    labelValue("SP total:  ", std::to_string(sp)),
                    labelValue("Syn:       ", synStr),
                    labelValue("Hörsel:    ", horStr),
                    text(""),
                });
            }(),
            text("Special Ability") | bold | color(kHeader),
            separator() | color(kDim),
            c.hasSpecialAbility
                ? paragraph(c.specialAbilityDesc) | color(kAccent)
                : text("None (skipped)") | color(kDim),
            text(""),
            text("Skills (" + std::to_string(c.skills.size()) + ")") | bold | color(kHeader),
            separator() | color(kDim),
            vbox(skillEls),
            filler(),
            separator() | color(kDim),
            // Unspent BP / EP warnings
            [&]() -> Element {
                Elements warnings;
                int unspentBP = state.bpRemaining();
                int unspentEP = state.epAvailable();
                if (unspentBP > 0)
                    warnings.push_back(
                        text("  ⚠ " + std::to_string(unspentBP) + " BP unspent — consider spending before saving.")
                        | color(kError) | bold);
                if (unspentEP > 0)
                    warnings.push_back(
                        text("  ⚠ " + std::to_string(unspentEP) + " EP unspent — use EP spending in Skills.")
                        | color(kError) | bold);
                if (warnings.empty()) return text("");
                return vbox(warnings);
            }(),
            text("Save character") | bold | color(kHeader),
            hbox({
                text("Filename: ") | color(kDim),
                filenameInput->Render() | size(WIDTH, EQUAL, 30) | border,
            }),
            saveMsg->empty() ? text("") :
                text("  " + *saveMsg) | color(*saveMsgOk ? kSuccess : kError) | bold,
            text(""),
            keyBar(state.isReadyToSave()
                ? std::vector<std::pair<std::string,std::string>>{{"Ctrl+S","Save JSON"}, {"Esc","Back to Dashboard"}}
                : std::vector<std::pair<std::string,std::string>>{{"Esc","Back (complete character to save)"}}),
        });

        return vbox({
            headerBar(state.isReadyToSave() ? "Summary & Save  ✓ Ready" : "Summary & Save  — incomplete"),
            hbox({
                leftPanel  | size(WIDTH, EQUAL, 45) ,
                separator() | color(kDim),
                rightPanel | flex ,
            }) | flex,
        }) | border;
    });

    return CatchEvent(renderer, [&app, layout, doSave](Event e) -> bool {
        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (e == Event::Special("\x13")) {  // Ctrl+S
            if (app.state().isReadyToSave())
                doSave();
            return true;
        }
        return layout->OnEvent(e);
    });
}

} // namespace dod
