cmake_minimum_required(VERSION 3.25)
project(game_hander)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required libs
find_package(OpenCV REQUIRED)
find_package(SDL2 REQUIRED)

add_library(
    game_hander
    src/Card.cpp
    src/CardFactory.cpp
    src/Game.cpp
    src/Hand.cpp
    src/Player.cpp
    src/Round.cpp
    src/RoundAction.cpp
)

target_include_directories(game_hander PUBLIC include src)

target_link_libraries(
    game_hander
    ${OpenCV_LIBS}
    ${SDL2_LIBRARIES}
)

add_executable(main main.cpp)

# Add the test subdirectory for building tests
if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()
