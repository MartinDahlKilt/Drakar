#pragma once
#include "core/Character.h"
#include <nlohmann/json.hpp>
#include <string>

namespace dod {

/// Serialises a Character to nlohmann::json and can persist it to disk.
class CharacterSerializer {
public:
    /// Convert a Character to a JSON object.
    static nlohmann::json toJson(const Character& c);

    /// Write the character as formatted JSON to `filepath`.
    /// @throws std::runtime_error if the file cannot be opened.
    static void saveToFile(const Character& c, const std::string& filepath);
};

} // namespace dod
