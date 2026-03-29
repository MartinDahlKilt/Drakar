#include "SynHorselScreen.h"
#include "../TuiTheme.h"

#include "core/Dice.h"
#include "core/GameRules.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <algorithm>
#include <string>

using namespace ftxui;

namespace dod {

/// Map a 2T6 roll (before BP bonus) + BP bonus total to a modifier.
/// Table: 1=-2, 2=-1, 3-4=0, 5-6=+1, 7-9=+2, 10+= +3
static int synHorselMod(int total) {
    if (total <= 1) return -2;
    if (total <= 2) return -1;
    if (total <= 4) return  0;
    if (total <= 6) return +1;
    if (total <= 9) return +2;
    return +3;
}

static std::string synHorselLabel(int mod) {
    switch (mod) {
        case -2: return "Dålig";
        case -1: return "Nedsatt";
        case  0: return "Normal";
        case +1: return "God";
        case +2: return "Mycket god";
        case +3: return "Utmärkt";
        default: return "?";
    }
}

// ---------------------------------------------------------------------------
// Two-stage sub-screen (Syn then Hörsel).
// stage 0 = rolling Syn, stage 1 = rolling Hörsel, stage 2 = done.
// ---------------------------------------------------------------------------
ftxui::Component MakeSynHorselScreen(TuiApp& app) {
    using namespace theme;

    // Per-stage state
    struct Stage {
        int  bpBonus    = 0;
        bool hasRolled  = false;
        int  rollTotal  = 0;
        std::vector<int> dice;
        int  modifier   = 0;
        std::string statusMsg;
    };

    auto stages   = std::make_shared<std::array<Stage, 2>>();
    auto stageIdx = std::make_shared<int>(0);

    // If already completed, show locked state
    {
        auto& c = app.state().character;
        if (app.state().isComplete(Section::SynHorsel)) {
            (*stages)[0].hasRolled = true;
            (*stages)[0].rollTotal = c.synRoll;
            (*stages)[0].modifier  = c.synMod;
            (*stages)[1].hasRolled = true;
            (*stages)[1].rollTotal = c.horselRoll;
            (*stages)[1].modifier  = c.horselMod;
            *stageIdx = 2;
        }
    }

    auto doRoll = [&app, stages, stageIdx]() {
        int idx = *stageIdx;
        if (idx >= 2) return;
        Stage& s = (*stages)[idx];
        if (s.hasRolled) return;

        auto& state = app.state();
        int availBP = state.bpRemaining();
        int bp = std::max(0, std::min(s.bpBonus, availBP));
        if (bp < s.bpBonus) {
            s.statusMsg = "Not enough BP — using " + std::to_string(bp) + " BP.";
            s.bpBonus = bp;
        }

        auto result = app.dice().roll(2, 6, bp);
        s.dice      = result.rolls;
        s.rollTotal = result.total;
        s.modifier  = synHorselMod(result.total);
        s.hasRolled = true;
        s.statusMsg = "";

        // Spend BP
        if (idx == 0) state.bpBreakdown.forSyn    = bp;
        else          state.bpBreakdown.forHorsel  = bp;

        // Record in roll history
        RollRecord rec;
        rec.context  = (idx == 0) ? "Syn (Sight)" : "Hörsel (Hearing)";
        rec.diceExpr = "2T6" + (bp != 0 ? ("+" + std::to_string(bp)) : "");
        rec.rolls    = result.rolls;
        rec.modifier = bp;
        rec.total    = result.total;
        rec.outcome  = synHorselLabel(s.modifier) + " (" + (s.modifier >= 0 ? "+" : "")
                     + std::to_string(s.modifier) + ")";
        state.character.rollHistory.push_back(rec);

        // Commit immediately — no separate confirm step
        if (idx == 0) {
            state.character.synRoll = s.rollTotal;
            state.character.synMod  = s.modifier;
            *stageIdx = 1;  // advance to Hearing
        } else {
            state.character.horselRoll = s.rollTotal;
            state.character.horselMod  = s.modifier;
            *stageIdx = 2;
            state.markComplete(Section::SynHorsel);
            // Stay on screen so the user can review both results before pressing Esc
        }
    };

    auto layout = Container::Vertical({});

    auto renderer = Renderer(layout, [&app, stages, stageIdx, layout]() -> Element {
        auto& state = app.state();
        int idx = *stageIdx;

        auto bpLine = [&]() {
            return hbox({
                text("  BP remaining: ") | color(kDim),
                text(std::to_string(state.bpRemaining())) | bold | color(kAccent),
            });
        };

        auto tableEl = []() {
            return vbox({
                text("  Roll table (2T6 + BP bonus):") | color(kDim),
                text("  1       → Dålig    (-2)") | color(kDim),
                text("  2       → Nedsatt  (-1)") | color(kDim),
                text("  3-4     → Normal   (±0)") | color(kDim),
                text("  5-6     → God      (+1)") | color(kDim),
                text("  7-9     → Mycket god (+2)") | color(kDim),
                text("  10+     → Utmärkt  (+3)") | color(kDim),
            });
        };

        auto stageEl = [&](int i, const char* title, const char* effect) -> Element {
            Stage& s = (*stages)[i];
            bool active = (idx == i);

            Element header = text(std::string("  ") + title) | bold
                           | color(active ? kTitle : (s.hasRolled ? kSuccess : kDim));

            if (s.hasRolled) {
                int diceSum = s.dice.empty() ? s.rollTotal : 0;
                for (int d : s.dice) diceSum += d;
                int mod = s.dice.empty() ? 0 : (s.rollTotal - diceSum);

                Elements calcEls;
                if (s.dice.empty()) {
                    calcEls.push_back(text("(locked)") | color(kDim));
                } else {
                    for (int d : s.dice)
                        calcEls.push_back(text(" [" + std::to_string(d) + "] ") | bold | color(kTitle));
                    if (mod != 0)
                        calcEls.push_back(text((mod >= 0 ? " +" : " ") + std::to_string(mod)) | color(kAccent));
                    calcEls.push_back(text(" = " + std::to_string(s.rollTotal)) | bold | color(kAccent));
                }

                return vbox({
                    header,
                    hbox(calcEls),
                    hbox({
                        text("  Result: ") | color(kDim),
                        text(synHorselLabel(s.modifier)) | bold | color(kValue),
                        text("  (") | color(kDim),
                        text((s.modifier >= 0 ? "+" : "") + std::to_string(s.modifier)) | bold
                            | color(s.modifier >= 0 ? kSuccess : kError),
                        text(" " + std::string(effect) + ")") | color(kDim),
                    }),
                });
            }

            if (active) {
                int bp = s.bpBonus;
                return vbox({
                    header,
                    text(std::string("  Affects: ") + effect) | color(kDim),
                    text(""),
                    hbox({
                        text("  BP bonus: ") | color(kDim),
                        text("← ") | color(kDim),
                        text(std::to_string(bp)) | bold | color(kAccent) | size(WIDTH, EQUAL, 4),
                        text(" → ") | color(kDim),
                        text("(max " + std::to_string(state.bpRemaining()) + ")") | color(kDim),
                    }),
                    s.statusMsg.empty() ? text("") : text("  " + s.statusMsg) | color(kError),
                });
            }

            return hbox({ header, text("  (not yet rolled)") | color(kDim) });
        };

        Element body;
        if (idx >= 2) {
            // Done — show both results
            body = vbox({
                text("  Both rolls complete.") | bold | color(kSuccess),
                text(""),
                stageEl(0, "Syn (Sight)",     "FV modifier to Finna dolda ting & Uptäcka fara"),
                text(""),
                stageEl(1, "Hörsel (Hearing)", "FV modifier to Upptäcka fara"),
            });
        } else {
            body = vbox({
                tableEl(),
                text(""),
                bpLine(),
                text(""),
                stageEl(0, "Syn (Sight)",     "FV to Finna dolda ting & Upptäcka fara"),
                text(""),
                stageEl(1, "Hörsel (Hearing)", "FV to Upptäcka fara"),
            });
        }

        return vbox({
            headerBar("Syn & Hörsel — Krigarexpansion"),
            vbox({
                text(""),
                body,
                filler(),
                separator() | color(kDim),
                idx >= 2
                    ? keyBar({{"Esc","Back to Dashboard"}})
                    : keyBar({{"←→","Adjust BP bonus"}, {"R","Roll"}, {"Esc","Back"}}),
            }) | flex,
        }) | border;
    });

    return CatchEvent(renderer, [&app, stages, stageIdx, doRoll](Event e) -> bool {
        int idx = *stageIdx;

        if (e == Event::Escape) {
            app.navigate(Section::Dashboard);
            return true;
        }
        if (idx >= 2) return false;

        Stage& s = (*stages)[idx];

        if (!s.hasRolled) {
            if (e == Event::ArrowLeft) {
                s.bpBonus = std::max(0, s.bpBonus - 1);
                return true;
            }
            if (e == Event::ArrowRight) {
                s.bpBonus = std::min(s.bpBonus + 1, app.state().bpRemaining());
                return true;
            }
            if (e == Event::Character('r') || e == Event::Character('R')) {
                doRoll();
                return true;
            }
        }
        return false;
    });
}

} // namespace dod
