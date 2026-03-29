add_executable(dod_tests
    test_dice.cpp
    test_gamerules.cpp
    test_gamedata.cpp
    test_serializer.cpp
)

target_link_libraries(dod_tests
    PRIVATE dod_core
    PRIVATE GTest::gtest_main
)

target_compile_options(dod_tests PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8 /WX>
)

include(GoogleTest)
gtest_discover_tests(dod_tests)
