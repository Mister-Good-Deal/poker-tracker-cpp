cmake_minimum_required(VERSION 3.24)

project(scraper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-----------------------------------------------------------------------------------------------------------------------
# Libs dependencies
#-----------------------------------------------------------------------------------------------------------------------

find_package(OpenCV REQUIRED)

#-----------------------------------------------------------------------------------------------------------------------
# Source files definition
#-----------------------------------------------------------------------------------------------------------------------

set(
    SRC
    src/ScraperInterface.cpp
    src/WinamaxScraper.cpp
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

set(COMMON_LIBS logger game_handler opencv_core opencv_imgproc)

if (WIN32)
    target_link_libraries(scraper PUBLIC user32 opencv_highgui ${COMMON_LIBS})
elseif (LINUX)
    target_link_libraries(scraper PUBLIC X11 Xext ${COMMON_LIBS})
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

if (WIN32)
    install(
        FILES $<TARGET_FILE:opencv_core> $<TARGET_FILE:opencv_imgproc> $<TARGET_FILE:opencv_highgui>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(
        FILES ${CMAKE_FIND_ROOT_PATH}/lib/libuser32.a
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
elseif (LINUX)
    install(
        FILES $<TARGET_FILE:opencv_core> $<TARGET_FILE:opencv_imgproc> /usr/lib/x86_64-linux-gnu/libX11.so /usr/lib/x86_64-linux-gnu/libXext.so
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif ()
