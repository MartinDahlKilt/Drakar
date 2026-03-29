#include "core/Dice.h"
#include <stdexcept>
#include <sstream>
#include <numeric>

namespace dod {

// ---- DiceResult ----

std::string DiceResult::describe() const {
    std::ostringstream oss;
    oss << count << "T" << sides;
    if (modifier != 0) {
        oss << (modifier > 0 ? "+" : "") << modifier;
    }
    oss << ": [";
    for (std::size_t i = 0; i < rolls.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << rolls[i];
    }
    oss << "] = " << total;
    return oss.str();
}

// ---- DiceRoller ----

DiceRoller::DiceRoller(unsigned int seed) : rng_(seed) {}

DiceRoller::DiceRoller() : rng_(std::random_device{}()) {}

DiceResult DiceRoller::roll(int count, int sides, int modifier) {
    if (count < 1)  throw std::invalid_argument("count must be >= 1");
    if (sides < 2)  throw std::invalid_argument("sides must be >= 2");

    std::uniform_int_distribution<int> dist(1, sides);
    DiceResult result;
    result.count    = count;
    result.sides    = sides;
    result.modifier = modifier;

    for (int i = 0; i < count; ++i) {
        result.rolls.push_back(dist(rng_));
    }
    result.total = std::accumulate(result.rolls.begin(), result.rolls.end(), 0) + modifier;
    return result;
}

DiceResult DiceRoller::rollPercent() {
    // Two T10 dice: first = units (1-10), second = tens (0-9)*10; "00" = 100
    std::uniform_int_distribution<int> d10(0, 9);
    int units = d10(rng_) + 1;     // 1-10
    int tens  = d10(rng_) * 10;    // 0, 10, 20 … 90

    DiceResult result;
    result.count    = 2;
    result.sides    = 10;
    result.modifier = 0;
    result.rolls    = {units, tens};
    result.total    = (tens + units == 0) ? 100 : (tens + units > 100 ? 100 : tens + units);

    // Simpler: tens digit 0-9, units digit 1-10 → range 1-100
    int raw = tens + units;
    result.total = (raw == 0 || raw > 100) ? 100 : raw;
    return result;
}

DiceResult DiceRoller::rollT3() {
    auto base  = roll(1, 6);
    int  value = (base.total <= 2) ? 1 : (base.total <= 4) ? 2 : 3;
    DiceResult result;
    result.count    = 1;
    result.sides    = 3;
    result.modifier = 0;
    result.rolls    = {base.total};  // show the T6 face that was rolled
    result.total    = value;
    return result;
}

DiceResult DiceRoller::rollT2() {
    auto base  = roll(1, 6);
    int  value = (base.total <= 3) ? 1 : 2;
    DiceResult result;
    result.count    = 1;
    result.sides    = 2;
    result.modifier = 0;
    result.rolls    = {base.total};
    result.total    = value;
    return result;
}

} // namespace dod
