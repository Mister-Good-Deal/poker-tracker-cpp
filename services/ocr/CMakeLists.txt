cmake_minimum_required(VERSION 3.24)

project(ocr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-----------------------------------------------------------------------------------------------------------------------
# Libs dependencies
#-----------------------------------------------------------------------------------------------------------------------

find_package(OpenCV CONFIG REQUIRED)

#-----------------------------------------------------------------------------------------------------------------------
# Source files definition
#-----------------------------------------------------------------------------------------------------------------------

set(
        SRC
        src/OcrFactory.cpp
        src/OcrInterface.cpp
        src/WinamaxOcr.cpp
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
        utilities
        game_handler
)

target_link_libraries(ocr PUBLIC logger utilities game_handler opencv_core opencv_imgproc opencv_text)

#-----------------------------------------------------------------------------------------------------------------------
# Resources and constants
#-----------------------------------------------------------------------------------------------------------------------

# Add the image file as a resource
set(WINAMAX_IMAGES_DIR "${PROJECT_SOURCE_DIR}/resources/winamax")

add_definitions(-DWINAMAX_IMAGES_DIR="${WINAMAX_IMAGES_DIR}")

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

if (WIN32)
    install(
            FILES $<TARGET_FILE:opencv_core> $<TARGET_FILE:opencv_imgproc> $<TARGET_FILE:opencv_text>
            $<TARGET_FILE:opencv_ml> $<TARGET_FILE:opencv_dnn>
            DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
elseif (LINUX)
    install(
            FILES $<TARGET_FILE:opencv_core> $<TARGET_FILE:opencv_imgproc> $<TARGET_FILE:opencv_text>
            $<TARGET_FILE:opencv_ml> $<TARGET_FILE:opencv_dnn>
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif ()
