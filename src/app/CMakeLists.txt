file(GLOB_RECURSE TUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/tui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tui/screens/*.cpp"
)

add_executable(dod_creator
    main.cpp
    ${TUI_SOURCES}
)

target_include_directories(dod_creator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dod_creator
    PRIVATE
        dod_core
        ftxui::screen
        ftxui::dom
        ftxui::component
)

target_compile_options(dod_creator PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8 /WX>
)

# On Windows the executable must stay a console app (TUI requires a terminal)
if(WIN32)
    set_target_properties(dod_creator PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()
