add_library(dod_core STATIC
    src/Dice.cpp
    src/GameData.cpp
    src/GameRules.cpp
    src/CharacterSerializer.cpp
)

target_include_directories(dod_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(dod_core
    PUBLIC nlohmann_json::nlohmann_json
)

target_compile_options(dod_core PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8 /WX>
)
