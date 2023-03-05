cmake_minimum_required(VERSION 3.24)

project(scraper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-----------------------------------------------------------------------------------------------------------------------
# Libs dependencies
#-----------------------------------------------------------------------------------------------------------------------

find_package(OpenCV REQUIRED)
find_package(SDL2 REQUIRED)

#-----------------------------------------------------------------------------------------------------------------------
# Source files definition
#-----------------------------------------------------------------------------------------------------------------------

set(
    SRC
    src/ScraperInterface.cpp
    src/WinamaxScraper.cpp
)

set(
    HEADERS
    include/ScraperInterface.hpp
    include/WinamaxScraper.hpp
)

#-----------------------------------------------------------------------------------------------------------------------
# Library target creation and setup
#-----------------------------------------------------------------------------------------------------------------------

add_library(scraper SHARED ${SRC})

target_include_directories(
    scraper
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
    logger
    game_handler
)

set(COMMON_LIBS logger game_handler opencv_core opencv_imgproc ${SDL2_LIBRARIES})

if (WIN32)
    target_link_libraries(scraper PRIVATE user32 opencv_highgui ${COMMON_LIBS})
elseif (LINUX)
    target_link_libraries(scraper PRIVATE X11 Xext ${COMMON_LIBS})
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Tests
#-----------------------------------------------------------------------------------------------------------------------

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------------------------------------------------------

install(
    TARGETS scraper
    EXPORT scraper_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/scraper
)

install(
    EXPORT scraper_targets
    FILE scraper_targets.cmake
    NAMESPACE Scraper::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/scraper
)

