#pragma once
#include <vector>
#include <random>
#include <string>

namespace dod {

/// Result of a single dice roll expression (e.g. 2T6+2)
struct DiceResult {
    int              total;     ///< Final sum including modifier
    std::vector<int> rolls;     ///< Individual die values before modifier
    int              modifier;  ///< The flat modifier that was added
    int              sides;     ///< Sides of the die that was rolled
    int              count;     ///< Number of dice rolled

    /// Human-readable description: "2T6+2: [3, 5] = 10"
    std::string describe() const;
};

/// Responsible for all random dice rolls in the application.
/// A single DiceRoller instance should be shared throughout a session.
class DiceRoller {
public:
    /// Construct with an explicit seed (useful for testing).
    explicit DiceRoller(unsigned int seed);

    /// Construct with a non-deterministic seed from std::random_device.
    DiceRoller();

    /// Roll `count` dice each with `sides` sides, optionally add `modifier`.
    /// @throws std::invalid_argument if count < 1 or sides < 2.
    DiceResult roll(int count, int sides, int modifier = 0);

    /// Roll 1T100 (two T10 dice: tens then units, "00" = 100).
    DiceResult rollPercent();

    /// Simulate a T3: roll 1T6, map 1-2→1, 3-4→2, 5-6→3.
    DiceResult rollT3();

    /// Simulate a T2: roll 1T6, map 1-3→1, 4-6→2.
    DiceResult rollT2();

private:
    std::mt19937 rng_;
};

} // namespace dod
