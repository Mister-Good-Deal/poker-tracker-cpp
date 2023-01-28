cmake_minimum_required(VERSION 3.25)
project(poker_bot)

if (BUILD_TESTS)
    enable_testing()
endif ()

add_subdirectory(poker_game)
