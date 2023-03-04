cmake_minimum_required(VERSION 3.24)
project(game_handler LANGUAGES CXX)

# Make cache variables for install destinations
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required libs
# logger target is automatically imported by parent project call
find_package(nlohmann_json REQUIRED)

# Source and header files
set(
    SRC
    src/Board.cpp
    src/Card.cpp
    src/CardFactory.cpp
    src/Game.cpp
    src/Hand.cpp
    src/Player.cpp
    src/Round.cpp
    src/RoundAction.cpp
)

set(
    HEADERS
    include/Board.hpp
    include/Card.hpp
    include/CardFactory.hpp
    include/Game.hpp
    include/Hand.hpp
    include/Player.hpp
    include/Round.hpp
    include/RoundAction.hpp
)

# Create the library
add_library(game_handler SHARED ${SRC})

target_include_directories(
    game_handler
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
    logger
)

target_link_libraries(game_handler PRIVATE logger nlohmann_json::nlohmann_json)

install(
    TARGETS game_handler
    EXPORT game_handler_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT game_handler_targets
    FILE game_handler_targets.cmake
    NAMESPACE GameHandler::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GameHandler
)

# To use the exported GameHandler library in other projects use:
# include(${INSTALL_PREFIX}/lib/cmake/game_handler_targets.cmake)
# ...
# target_link_libraries(someTarget PRIVATE game_handler::game_handler)

# Add the test subdirectory for building tests
if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()
