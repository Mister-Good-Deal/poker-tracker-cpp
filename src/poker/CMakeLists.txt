cmake_minimum_required(VERSION 3.25)
project(poker_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check the system type
if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(CMAKE_GENERATOR "MinGW Makefiles")
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
else ()
    # Default to native build
    set(CMAKE_GENERATOR "Unix Makefiles")
endif ()

# Required libs
find_package(OpenCV REQUIRED)
find_package(SDL2 REQUIRED)

add_library(
    poker_game
    Card.cpp
    Player.cpp
    Round.cpp
)

target_include_directories(poker_game PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(
    poker_game
    ${OpenCV_LIBS}
    ${SDL2_LIBRARIES}
)

add_executable(main.cpp main)

# Add the test subdirectory for building tests
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()