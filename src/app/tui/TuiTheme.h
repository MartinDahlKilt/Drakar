#pragma once
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

namespace dod::theme {

using namespace ftxui;

// ---------------------------------------------------------------------------
// Color palette
// ---------------------------------------------------------------------------
inline const Color kHeader      = Color::Cyan;
inline const Color kAccent      = Color::Yellow;
inline const Color kSuccess     = Color::Green;
inline const Color kWarning     = Color::Yellow;
inline const Color kError       = Color::Red;
inline const Color kDim         = Color::GrayDark;
inline const Color kValue       = Color::White;
inline const Color kBPColor     = Color::Magenta;
inline const Color kTitle       = Color::CyanLight;

// ---------------------------------------------------------------------------
// Shared element builders
// ---------------------------------------------------------------------------

/// Full-width title bar at the top of every screen.
inline Element headerBar(const std::string& title) {
    return hbox({
        text(" DRAKAR OCH DEMONER ") | bold | color(kTitle),
        text("— ") | color(kDim),
        text(title) | bold | color(kHeader),
        filler(),
    }) | bgcolor(Color::Black) | underlined;
}

/// BP status bar showing remaining/total with a visual fill.
inline Element bpBar(int remaining, int total) {
    int spent = total - remaining;
    double frac = (total > 0) ? (double)spent / total : 0.0;
    int barWidth = 20;
    int filled = (int)(frac * barWidth + 0.5);
    if (filled < 0) filled = 0;
    if (filled > barWidth) filled = barWidth;
    int empty = barWidth - filled;

    return hbox({
        text(" BP: ") | color(kDim),
        text(std::to_string(remaining)) | bold | color(kBPColor),
        text(" / " + std::to_string(total)) | color(kDim),
        text("  [") | color(kDim),
        text(std::string(filled, '#')) | color(kBPColor),
        text(std::string(empty, '-')) | color(kDim),
        text("]") | color(kDim),
        text("  (spent: " + std::to_string(spent) + ")") | color(kDim),
    });
}

/// A colored badge / pill of text.
inline Element badge(const std::string& text_, Color c) {
    return text(" " + text_ + " ") | color(c) | bold;
}

/// Section status indicator: ✓ green / ○ yellow (in-progress) / · dim (not started).
inline Element sectionStatus(bool complete, bool visited) {
    if (complete)  return text("✓") | color(kSuccess) | bold;
    if (visited)   return text("○") | color(kWarning);
    return text("·") | color(kDim);
}

/// A key-hint like "[Enter] Confirm  [Esc] Back"
inline Element keyHint(const std::string& key, const std::string& desc) {
    return hbox({
        text("[") | color(kDim),
        text(key) | bold | color(kAccent),
        text("] ") | color(kDim),
        text(desc) | color(kDim),
    });
}

/// Horizontal key hints bar.
inline Element keyBar(std::vector<std::pair<std::string,std::string>> hints) {
    Elements els;
    for (std::size_t i = 0; i < hints.size(); ++i) {
        if (i > 0) els.push_back(text("  "));
        els.push_back(keyHint(hints[i].first, hints[i].second));
    }
    return hbox(std::move(els));
}

/// Separator line.
inline Element sep() {
    return separator() | color(kDim);
}

/// A value row: "Label ........ Value"
inline Element labelValue(const std::string& label, const std::string& value,
                           Color vc = kValue) {
    return hbox({
        text(label) | color(kDim),
        filler(),
        text(value) | color(vc) | bold,
    });
}

/// Panel with a border and title.
inline Element panel(const std::string& title, Element content) {
    return window(text(" " + title + " ") | bold | color(kHeader),
                  std::move(content));
}

} // namespace dod::theme
