cmake_minimum_required(VERSION 3.24)

project(game_handler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-----------------------------------------------------------------------------------------------------------------------
# Libs dependencies
#-----------------------------------------------------------------------------------------------------------------------

find_package(nlohmann_json CONFIG REQUIRED)

#-----------------------------------------------------------------------------------------------------------------------
# Source files definition
#-----------------------------------------------------------------------------------------------------------------------

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

#-----------------------------------------------------------------------------------------------------------------------
# Library target creation and setup
#-----------------------------------------------------------------------------------------------------------------------

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

#-----------------------------------------------------------------------------------------------------------------------
# Tests
#-----------------------------------------------------------------------------------------------------------------------

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------------------------------------------------------

include(GNUInstallDirs)

install(
    TARGETS game_handler
    EXPORT game_handler_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/game_handler
)

install(
    EXPORT game_handler_targets
    FILE game_handler_targets.cmake
    NAMESPACE GameHandler::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/game_handler
)
