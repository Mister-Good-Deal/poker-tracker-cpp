cmake_minimum_required(VERSION 3.24)

project(game_session LANGUAGES CXX)

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
        src/Session.cpp
)

#-----------------------------------------------------------------------------------------------------------------------
# Library target creation and setup
#-----------------------------------------------------------------------------------------------------------------------

add_library(game_session SHARED ${SRC})

target_include_directories(
        game_session
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        PRIVATE
        logger
        game_handler
        scraper
        ocr
)

target_link_libraries(game_session PUBLIC logger game_handler scraper ocr nlohmann_json::nlohmann_json)

#-----------------------------------------------------------------------------------------------------------------------
# Resources and constants
#-----------------------------------------------------------------------------------------------------------------------

# Add the image file as a resource
set(WINAMAX_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/resources/winamax")

add_definitions(-DWINAMAX_RESOURCES_DIR="${WINAMAX_RESOURCES_DIR}")

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
        TARGETS game_session
        EXPORT game_session_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/game_session
)

install(
        EXPORT game_session_targets
        FILE game_session_targets.cmake
        NAMESPACE GameSession::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/game_session
)
