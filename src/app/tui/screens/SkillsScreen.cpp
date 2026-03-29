#include "SkillsScreen.h"
#include "../TuiTheme.h"

#include "core/GameData.h"
#include "core/GameRules.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <algorithm>

using namespace ftxui;

namespace dod {

static int getStatValS(const Stats& s, const std::string& n) {
    if (n == "STY") return s.STY;
    if (n == "FYS") return s.FYS;
    if (n == "SMI") return s.SMI;
    if (n == "INT") return s.INT;
    if (n == "PSY") return s.PSY;
    if (n == "KAR") return s.KAR;
    if (n == "STO") return s.STO;
    return 10;
}

// Number of UTF-8 code points in s
static size_t utf8Len(const std::string& s) {
    size_t count = 0;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        i += (c < 0x80u) ? 1u : (c < 0xE0u) ? 2u : (c < 0xF0u) ? 3u : 4u;
        ++count;
    }
    return count;
}

// Truncate s to maxChars UTF-8 code points
static std::string utf8Substr(const std::string& s, size_t maxChars) {
    size_t bytePos = 0;
    size_t charCount = 0;
    while (bytePos < s.size() && charCount < maxChars) {
        unsigned char c = static_cast<unsigned char>(s[bytePos]);
        bytePos += (c < 0x80u) ? 1u : (c < 0xE0u) ? 2u : (c < 0xF0u) ? 3u : 4u;
        ++charCount;
    }
    return s.substr(0, bytePos);
}

ftxui::Component MakeSkillsScreen(TuiApp& app) {
    using namespace theme;

    // -------------------------------------------------------------------------
    // Shared state
    // -------------------------------------------------------------------------
    // 0 = profession skill selection, 1 = EP spending
    auto viewMode = std::make_shared<int>(0);
    // Auto-switch to EP view if skills already complete on entry
    {
        auto& state = app.state();
        if (state.isComplete(Section::Skills)) *viewMode = 1;
    }

    // =========================================================================
    // VIEW 0 — PROFESSION SKILL SELECTION (existing logic, unchanged)
    // =========================================================================

    // Resolve profession
    std::optional<ProfessionDefinition> profOpt;
    {
        auto& state = app.state();
        if (!state.character.profession.empty())
            profOpt = GameData::findProfession(state.character.profession);
    }

    struct SkillOption {
        std::string label;
        std::string rawName;
        SkillId     skillId = SkillId::PLAYER_CHOICE;
        bool isWeapon = false, isLanguage = false, isLiteracy = false, isInstrument = false;
        std::string specifiedName;
    };

    int maxSkills = profOpt ? profOpt->maxSkillChoices : 12;
    int maxWeapon = profOpt ? profOpt->maxWeaponSkills : -1;
    int maxLang   = profOpt ? profOpt->maxLanguages    :  0;
    int maxLit    = profOpt ? profOpt->maxLiteracy     :  0;
    int maxInst   = profOpt ? profOpt->maxInstruments  :  0;

    auto options = std::make_shared<std::vector<SkillOption>>();
    if (profOpt) {
        for (const SkillId sid : profOpt->possibleSkills) {
            SkillOption opt;
            opt.skillId = sid;
            opt.rawName = skillIdToString(sid);
            opt.label   = opt.rawName;
            int copies  = 1;
            if (sid == SkillId::VAPENFARDIGHET) {
                opt.isWeapon = true; opt.label = "Weapon skill (choose type)";
                if (maxWeapon == 0) continue;
                copies = (maxWeapon == -1) ? maxSkills : maxWeapon;
            } else if (sid == SkillId::TALA_FRAMMANDE_SPRAK) {
                opt.isLanguage = true;
                if (maxLang <= 0) continue;
                copies = maxLang;
            } else if (sid == SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK) {
                opt.isLiteracy = true;
                if (maxLit <= 0) continue;
                copies = maxLit;
            } else if (sid == SkillId::SPELA_INSTRUMENT) {
                opt.isInstrument = true;
                if (maxInst == 0) continue;
                copies = (maxInst == -1) ? maxSkills : maxInst;
            }
            for (int k = 0; k < copies; ++k)
                options->push_back(opt);
        }
        std::sort(options->begin(), options->end(),
            [](const SkillOption& a, const SkillOption& b){ return a.label < b.label; });
    }

    auto focusRow  = std::make_shared<int>(0);
    auto selected  = std::make_shared<std::vector<bool>>(options->size(), false);
    auto specNames = std::make_shared<std::vector<std::string>>(options->size(), "");
    auto statusMsg = std::make_shared<std::string>("");
    auto inputMode = std::make_shared<bool>(false);
    auto inputStr  = std::make_shared<std::string>("");

    // Pre-populate from existing character skills (re-entry)
    {
        const auto& savedSkills = app.state().character.skills;
        std::vector<bool> matched(savedSkills.size(), false);
        for (int i = 0; i < (int)options->size(); ++i) {
            const auto& opt = (*options)[i];
            for (int j = 0; j < (int)savedSkills.size(); ++j) {
                if (matched[j]) continue;
                const auto& sk = savedSkills[j];
                if (!sk.isProfessionSkill) continue;
                bool fits = false;
                if (opt.isWeapon     && sk.name.rfind("Weapon: ", 0) == 0)    fits = true;
                else if (opt.isLanguage  && sk.name.rfind("Tala ", 0) == 0 && sk.name != opt.rawName) fits = true;
                else if (opt.isLiteracy  && sk.name.rfind("Läsa/Skriva ", 0) == 0 && sk.name != opt.rawName) fits = true;
                else if (opt.isInstrument && sk.name.rfind("Spela ", 0) == 0 && sk.name != opt.rawName) fits = true;
                else if (!opt.isWeapon && !opt.isLanguage && !opt.isLiteracy && !opt.isInstrument
                         && sk.name == opt.rawName) fits = true;
                if (fits) { (*selected)[i] = true; (*specNames)[i] = sk.name; matched[j] = true; break; }
            }
        }
    }

    InputOption inputOpt;
    inputOpt.placeholder = "Enter value…";
    auto textInput = Input(inputStr.get(), inputOpt);
    auto layout = Container::Vertical({textInput});

    auto countSelected = [selected]() -> int {
        int n = 0; for (bool b : *selected) if (b) ++n; return n;
    };

    auto applyAndSave = [&app, options, selected, specNames]() {
        auto& state = app.state();
        auto& skills = state.character.skills;
        skills.erase(std::remove_if(skills.begin(), skills.end(),
            [](const SkillEntry& e){ return e.isProfessionSkill; }), skills.end());

        auto finalStats = state.computeFinalStats();
        for (int i = 0; i < (int)options->size(); ++i) {
            if (!(*selected)[i]) continue;
            const auto& opt = (*options)[i];
            std::string finalName = (*specNames)[i].empty() ? opt.rawName : (*specNames)[i];
            std::string baseStat = "var.";
            int fv = 1;
            auto skillDef = GameData::findSkill(opt.skillId);
            if (skillDef) {
                baseStat = skillDef->baseStat;
                fv = GameRules::calculateBC(getStatValS(finalStats, baseStat));
            }
            // If SA already granted this skill as a secondary, promote it instead of duplicating
            auto saIt = std::find_if(skills.begin(), skills.end(),
                [&](const SkillEntry& e){ return e.name == finalName && !e.isPrimary && !e.isProfessionSkill; });
            if (saIt != skills.end()) {
                saIt->isProfessionSkill = true;
                if (saIt->fvBase < fv) saIt->fvBase = fv;
                if (saIt->fv    < fv) saIt->fv    = fv;
            } else {
                SkillEntry entry;
                entry.name              = finalName;
                entry.baseStat          = baseStat;
                entry.skillId           = skillDef ? skillDef->id : SkillId::PLAYER_CHOICE;
                entry.fv                = fv;
                entry.fvBase            = fv;
                entry.isPrimary         = false;
                entry.isProfessionSkill = true;
                skills.push_back(entry);
            }
        }

        // Rebuild primary skills — update existing entries (incl. SA-granted ones),
        // add missing ones. Do NOT erase-then-rebuild: that would lose SA bonuses.
        auto raceOpt = GameData::findRace(state.character.race);
        for (const auto& sk : GameData::getPrimarySkills()) {
            int sv = getStatValS(finalStats, sk.baseStat);
            int fv = GameRules::calculateBC(sv);
            if (raceOpt) {
                for (const auto& bonus : raceOpt->specialBonuses) {
                    if (bonus.skillId == sk.id)
                        fv = bonus.setFixed ? bonus.fvBonus : fv + bonus.fvBonus;
                }
            }
            // Find by name — may be an SA-granted entry (primary or secondary)
            auto it = std::find_if(skills.begin(), skills.end(),
                [&](const SkillEntry& e){ return e.name == skillIdToString(sk.id) && !e.isProfessionSkill; });
            if (it != skills.end()) {
                it->isPrimary         = true;
                it->isProfessionSkill = false;
                if (it->fvBase < fv) it->fvBase = fv;
                if (it->fv    < fv) it->fv    = fv;
            } else {
                SkillEntry entry;
                entry.name              = skillIdToString(sk.id);
                entry.baseStat          = sk.baseStat;
                entry.skillId           = sk.id;
                entry.fv                = fv;
                entry.fvBase            = fv;
                entry.isPrimary         = true;
                entry.isProfessionSkill = false;
                skills.push_back(entry);
            }
        }

        state.markComplete(Section::Skills);
    };

    auto toggleSelected = [options, focusRow, selected, specNames, statusMsg, inputMode, inputStr,
                           countSelected, maxSkills]() {
        int i = *focusRow;
        if (i >= (int)options->size()) return;
        auto& opt = (*options)[i];
        if ((*selected)[i]) {
            (*selected)[i] = false; (*specNames)[i] = ""; *statusMsg = ""; return;
        }
        if (countSelected() >= maxSkills) { *statusMsg = "Max " + std::to_string(maxSkills) + " skills reached."; return; }
        if (opt.isWeapon || opt.isLanguage || opt.isLiteracy || opt.isInstrument) {
            *inputMode = true; *inputStr = "";
            *statusMsg = opt.isWeapon ? "Enter weapon type (e.g. Sword, Axe, Bow):"
                       : opt.isLanguage ? "Enter language name:"
                       : opt.isLiteracy ? "Enter language for literacy:"
                       : "Enter instrument name:";
            return;
        }
        (*selected)[i] = true; (*specNames)[i] = opt.rawName; *statusMsg = "";
    };

    // =========================================================================
    // VIEW 1 — EP SPENDING
    // =========================================================================
    // focusCol: 0=Primary, 1=Profession, 2=Secondary
    auto epFocusCol    = std::make_shared<int>(0);
    auto epFocusRow    = std::make_shared<std::array<int,3>>(std::array<int,3>{0,0,0});
    auto epMsg         = std::make_shared<std::string>("");
    auto epSkillFocused = std::make_shared<bool>(false);

    // =========================================================================
    // RENDERER
    // =========================================================================
    auto renderer = Renderer(layout, [&app, viewMode, focusRow, selected, specNames, statusMsg,
                                       inputMode, inputStr, countSelected, textInput, layout, options,
                                       maxSkills, epFocusCol, epFocusRow, epMsg, epSkillFocused]() -> Element {
        auto& state = app.state();

        // --- VIEW 0: Profession skill selection ---
        if (*viewMode == 0) {
            if (options->empty()) {
                return vbox({
                    headerBar("Skills"),
                    vbox({
                        filler(),
                        text("  No profession selected.") | bold | color(kWarning),
                        text("  Please choose a profession first, then return here.") | color(kDim),
                        filler(),
                        keyBar({{"Esc","Back to Dashboard"}}),
                    }) | flex,
                }) | border;
            }

            auto finalStats = state.computeFinalStats();
            int sel = countSelected();
            Elements skillRows;
            for (int i = 0; i < (int)options->size(); ++i) {
                const auto& opt = (*options)[i];
                bool isSel   = (*selected)[i];
                bool focused = (i == *focusRow);
                std::string display = isSel
                    ? "[✓] " + ((*specNames)[i].empty() ? opt.label : (*specNames)[i])
                    : "[ ] " + opt.label;
                std::string bcStr;
                auto skillDef = GameData::findSkill(opt.rawName);
                if (skillDef) {
                    int sv = getStatValS(finalStats, skillDef->baseStat);
                    bcStr  = " BC=" + std::to_string(GameRules::calculateBC(sv))
                           + " (" + skillDef->baseStat + ")";
                }
                Element row = hbox({ text(display) | color(isSel ? kSuccess : kValue), text(bcStr) | color(kDim) });
                if (focused) row = row | bgcolor(Color::Blue) | color(Color::White);
                skillRows.push_back(row);
            }

            Elements selSummary;
            selSummary.push_back(text("Selected: " + std::to_string(sel) + " / "
                + std::to_string(maxSkills)) | bold | color(sel >= maxSkills ? kSuccess : kAccent));
            selSummary.push_back(separator() | color(kDim));
            for (int i = 0; i < (int)options->size(); ++i) {
                if (!(*selected)[i]) continue;
                std::string display = (*specNames)[i].empty() ? (*options)[i].label : (*specNames)[i];
                auto skillDef = GameData::findSkill((*options)[i].rawName);
                int fv = skillDef ? GameRules::calculateBC(getStatValS(finalStats, skillDef->baseStat)) : 0;
                selSummary.push_back(hbox({
                    text("  ✓ ") | color(kSuccess),
                    text(display) | color(kValue),
                    text("  FV=" + std::to_string(fv)) | color(kBPColor),
                }));
            }

            Element inputOverlay = vbox({});
            if (*inputMode) {
                inputOverlay = vbox({
                    text(""),
                    vbox({
                        text(*statusMsg) | bold | color(kAccent),
                        hbox({ text("  "), textInput->Render() | size(WIDTH, EQUAL, 30) | border }),
                        keyBar({{"Enter","Confirm"}, {"Esc","Cancel"}}),
                    }) | border | color(kWarning),
                });
            }

            Element body = vbox({
                headerBar("Skills — Select Profession Skills"),
                hbox({
                    vbox({
                        text(" Profession: " + state.character.profession) | bold | color(kHeader),
                        separator() | color(kDim),
                        vbox(skillRows) | flex,
                    }) | size(WIDTH, EQUAL, 50),
                    separator() | color(kDim),
                    vbox({
                        vbox(selSummary),
                        filler(),
                        separator() | color(kDim),
                        statusMsg->empty() || *inputMode ? text("") : text(*statusMsg) | color(kError),
                        text(""),
                        keyBar({{"Space/Enter","Toggle"}, {"Tab","EP Spending"}, {"Esc","Done / Back"}}),
                    }) | flex,
                }) | flex,
            }) | border;

            if (*inputMode) return dbox({body, inputOverlay | center | flex});
            return body;
        }

        // --- VIEW 1: EP spending ---
        auto finalStats = state.computeFinalStats();
        (void)finalStats; // used only when stat preview is added
        const auto& skills = state.character.skills;
        int fvCap = GameRules::getAgeFVCap(state.character.ageCategory);
        int epAvail = state.epAvailable();
        int epTotal = state.computeEPTotal();

        // Partition skills into three columns
        struct Col { std::string header; std::vector<const SkillEntry*> rows; };
        Col cols[3];
        cols[0].header = "Primary";
        cols[1].header = "Profession";
        cols[2].header = "Secondary";
        for (const auto& sk : skills) {
            if      (sk.isPrimary)         cols[0].rows.push_back(&sk);
            else if (sk.isProfessionSkill) cols[1].rows.push_back(&sk);
            else                           cols[2].rows.push_back(&sk);
        }
        for (auto& col : cols)
            std::sort(col.rows.begin(), col.rows.end(),
                [](const SkillEntry* a, const SkillEntry* b){ return a->name < b->name; });

        // Clamp focus indices
        for (int c = 0; c < 3; ++c)
            if (!cols[c].rows.empty())
                (*epFocusRow)[c] = std::max(0, std::min((*epFocusRow)[c], (int)cols[c].rows.size()-1));

        auto makeColElement = [&](int c) -> Element {
            const auto& col = cols[c];
            bool colFocused = (*epFocusCol == c);
            Elements rows;
            rows.push_back(text(" " + col.header) | bold | color(kHeader));
            rows.push_back(separator() | color(kDim));
            if (col.rows.empty()) {
                rows.push_back(text("  (none)") | color(kDim));
            } else {
                for (int r = 0; r < (int)col.rows.size(); ++r) {
                    const auto& sk = *col.rows[r];
                    bool focused = colFocused && (r == (*epFocusRow)[c]);
                    int nextCost = GameRules::getEPStepCost(sk.fv + 1, sk.isPrimary, sk.isProfessionSkill);
                    bool atCap   = (sk.fv >= fvCap);
                    bool canRaise = !atCap && epAvail >= nextCost;

                    std::string name = utf8Len(sk.name) > 22 ? utf8Substr(sk.name, 21)+"…" : sk.name;
                    Element row = hbox({
                        text(" " + name) | size(WIDTH, EQUAL, 24),
                        text(" FV:") | color(kDim),
                        text(std::to_string(sk.fv)) | bold | color(sk.fv > sk.fvBase ? kSuccess : kValue)
                            | size(WIDTH, EQUAL, 3),
                        sk.clNote.empty() ? text("") :
                            text(" [" + sk.clNote + "]") | color(kAccent) | size(WIDTH, EQUAL, 22),
                        atCap ? text(" (cap)") | color(kDim) :
                            text(" →" + std::to_string(nextCost) + "EP") | color(canRaise ? kBPColor : kDim),
                    });
                    if (focused && *epSkillFocused)
                        row = row | bgcolor(Color::Green) | color(Color::Black);
                    else if (focused)
                        row = row | bgcolor(Color::Blue) | color(Color::White);
                    rows.push_back(row);
                }
            }
            return vbox(rows) | flex;
        };

        return vbox({
            headerBar("Skills — Spend EP"),
            hbox({
                text("  EP available: ") | color(kDim),
                text(std::to_string(epAvail)) | bold | color(epAvail > 0 ? kSuccess : kError),
                text(" / " + std::to_string(epTotal)) | color(kDim),
                text("   Spent: ") | color(kDim),
                text(std::to_string(state.character.epSpent)) | bold | color(kAccent),
                text("   Age FV cap: ") | color(kDim),
                text(std::to_string(fvCap)) | bold | color(kValue),
            }),
            separator() | color(kDim),
            hbox({
                makeColElement(0) | flex,
                separator() | color(kDim),
                makeColElement(1) | flex,
                separator() | color(kDim),
                makeColElement(2) | flex,
            }) | flex,
            separator() | color(kDim),
            epMsg->empty() ? text("") : text("  " + *epMsg) | color(kError),
            *epSkillFocused
                ? keyBar({{"←→","Change FV"}, {"Enter/Esc","Defocus"}})
                : keyBar({{"↑↓","Navigate"}, {"←→","Switch Column"}, {"Enter","Focus Skill"}, {"Tab","Select Skills"}, {"Esc","Back"}}),
        }) | border;
    });

    // =========================================================================
    // EVENT HANDLER
    // =========================================================================
    return CatchEvent(renderer, [&app, viewMode, focusRow, selected, specNames, statusMsg,
                                  inputMode, inputStr, toggleSelected, applyAndSave,
                                  layout, options, textInput, epFocusCol, epFocusRow, epMsg,
                                  epSkillFocused](Event e) -> bool {

        // --- VIEW 0 events ---
        if (*viewMode == 0) {
            if (*inputMode) {
                if (e == Event::Escape) { *inputMode = false; *statusMsg = ""; return true; }
                if (e == Event::Return) {
                    if (!inputStr->empty()) {
                        int i = *focusRow;
                        auto& opt = (*options)[i];
                        std::string prefix;
                        if (opt.isWeapon)      prefix = "Weapon: ";
                        else if (opt.isLanguage)  prefix = "Tala ";
                        else if (opt.isLiteracy)  prefix = "Läsa/Skriva ";
                        else if (opt.isInstrument) prefix = "Spela ";
                        (*specNames)[i] = prefix + *inputStr;
                        (*selected)[i]  = true;
                        *inputMode = false; *statusMsg = "";
                    }
                    return true;
                }
                return textInput->OnEvent(e);
            }
            if (e == Event::Escape) { applyAndSave(); app.navigate(Section::Dashboard); return true; }
            if (e == Event::Tab) { applyAndSave(); *viewMode = 1; return true; }
            if (options->empty()) return false;
            if (e == Event::ArrowUp   || e == Event::Character('k')) {
                *focusRow = (*focusRow - 1 + (int)options->size()) % (int)options->size(); return true;
            }
            if (e == Event::ArrowDown || e == Event::Character('j')) {
                *focusRow = (*focusRow + 1) % (int)options->size(); return true;
            }
            if (e == Event::Character(' ') || e == Event::Return) { toggleSelected(); return true; }
            return false;
        }

        // --- VIEW 1 events ---
        auto& state = app.state();

        // Build column row-pointers (same logic as renderer, but mutable)
        std::vector<SkillEntry*> colRows[3];
        for (auto& sk : state.character.skills) {
            if      (sk.isPrimary)         colRows[0].push_back(&sk);
            else if (sk.isProfessionSkill) colRows[1].push_back(&sk);
            else                           colRows[2].push_back(&sk);
        }
        for (auto& col : colRows)
            std::sort(col.begin(), col.end(),
                [](SkillEntry* a, SkillEntry* b){ return a->name < b->name; });

        int c = *epFocusCol;
        int r = (*epFocusRow)[c];

        if (e == Event::Tab) { *epSkillFocused = false; *viewMode = 0; return true; }
        if (e == Event::Escape) {
            if (*epSkillFocused) { *epSkillFocused = false; *epMsg = ""; return true; }
            app.navigate(Section::Dashboard); return true;
        }

        if (e == Event::ArrowUp) {
            *epSkillFocused = false;
            if (!colRows[c].empty())
                (*epFocusRow)[c] = (r - 1 + (int)colRows[c].size()) % (int)colRows[c].size();
            return true;
        }
        if (e == Event::ArrowDown) {
            *epSkillFocused = false;
            if (!colRows[c].empty())
                (*epFocusRow)[c] = (r + 1) % (int)colRows[c].size();
            return true;
        }

        if (e == Event::ArrowLeft) {
            if (*epSkillFocused) {
                // Refund one FV step from the focused skill
                if (colRows[c].empty()) return true;
                SkillEntry& sk = *colRows[c][r];
                if (sk.fv <= sk.fvBase) { *epMsg = sk.name + " is already at base FV."; return true; }
                int refund = GameRules::getEPStepCost(sk.fv, sk.isPrimary, sk.isProfessionSkill);
                sk.fv--;
                state.character.epSpent -= refund;
                *epMsg = sk.name + " refunded to FV " + std::to_string(sk.fv) + " (+" + std::to_string(refund) + " EP returned).";
            } else {
                // Step left, skipping empty columns
                for (int next = *epFocusCol - 1; next >= 0; --next) {
                    if (!colRows[next].empty()) { *epFocusCol = next; *epMsg = ""; break; }
                }
            }
            return true;
        }
        if (e == Event::ArrowRight) {
            if (*epSkillFocused) {
                // Raise FV of the focused skill
                if (colRows[c].empty()) return true;
                SkillEntry& sk = *colRows[c][r];
                int fvCap = GameRules::getAgeFVCap(state.character.ageCategory);
                if (sk.fv >= fvCap) { *epMsg = sk.name + " is at the age FV cap (" + std::to_string(fvCap) + ")."; return true; }
                int cost = GameRules::getEPStepCost(sk.fv + 1, sk.isPrimary, sk.isProfessionSkill);
                if (state.epAvailable() < cost) {
                    *epMsg = "Not enough EP (need " + std::to_string(cost) + ", have " + std::to_string(state.epAvailable()) + ").";
                    return true;
                }
                sk.fv++;
                state.character.epSpent += cost;
                *epMsg = sk.name + " raised to FV " + std::to_string(sk.fv) + " (cost: " + std::to_string(cost) + " EP).";
            } else {
                // Step right, skipping empty columns
                for (int next = *epFocusCol + 1; next < 3; ++next) {
                    if (!colRows[next].empty()) { *epFocusCol = next; *epMsg = ""; break; }
                }
            }
            return true;
        }

        // Enter = toggle skill focus for FV editing
        if (e == Event::Return) {
            if (!colRows[c].empty()) { *epSkillFocused = !*epSkillFocused; *epMsg = ""; }
            return true;
        }
        // Backspace = quick refund without needing to focus
        if (e == Event::Backspace) {
            if (colRows[c].empty()) return true;
            SkillEntry& sk = *colRows[c][r];
            if (sk.fv <= sk.fvBase) { *epMsg = sk.name + " is already at base FV."; return true; }
            int refund = GameRules::getEPStepCost(sk.fv, sk.isPrimary, sk.isProfessionSkill);
            sk.fv--;
            state.character.epSpent -= refund;
            *epMsg = sk.name + " refunded to FV " + std::to_string(sk.fv) + " (+" + std::to_string(refund) + " EP returned).";
            return true;
        }

        return false;
    });
}
} // namespace dod
