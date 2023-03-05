cmake_minimum_required(VERSION 3.24)

project(ocr LANGUAGES CXX)

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
    src/OcrInterface.cpp
    src/WinamaxOcr.cpp
)

set(
    HEADERS
    include/OcrInterface.hpp
    include/WinamaxOcr.hpp
)

#-----------------------------------------------------------------------------------------------------------------------
# Library target creation and setup
#-----------------------------------------------------------------------------------------------------------------------

add_library(ocr SHARED ${SRC})

target_include_directories(
    ocr
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
    logger
    game_handler
)

target_link_libraries(ocr PRIVATE logger game_handler opencv_core opencv_imgproc opencv_text)

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
    TARGETS ocr
    EXPORT ocr_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ocr
)

install(
    EXPORT ocr_targets
    FILE ocr_targets.cmake
    NAMESPACE OCR::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ocr
)
