cmake_minimum_required(VERSION 3.25)

if (BUILD_TESTS)
    enable_testing()
endif ()

# Check the system type
if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
    set(CMAKE_GENERATOR "MinGW Makefiles")
else ()
    # Default to native build
    set(CMAKE_GENERATOR "Unix Makefiles")
endif ()

project(poker_bot)

add_subdirectory(services/game_handler)
