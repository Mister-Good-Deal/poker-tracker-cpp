cmake_minimum_required(VERSION 3.24)

project(utilities LANGUAGES CXX)

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
        include/utilities/Macros.hpp
)

#-----------------------------------------------------------------------------------------------------------------------
# Library target creation and setup
#-----------------------------------------------------------------------------------------------------------------------

add_library(utilities SHARED ${SRC})

target_include_directories(
        utilities
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        PRIVATE
        logger
)

target_link_libraries(utilities PUBLIC logger opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui)

# @todo needed until no .cpp file extension in SRC files
set_target_properties(utilities PROPERTIES LINKER_LANGUAGE CXX)

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
        TARGETS utilities
        EXPORT utilities_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/utilities
)

install(
        EXPORT utilities_targets
        FILE utilities_targets.cmake
        NAMESPACE Utilities::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/utilities
)

install(
        FILES $<TARGET_FILE:opencv_core> $<TARGET_FILE:opencv_imgproc> $<TARGET_FILE:opencv_imgcodecs>
        $<TARGET_FILE:opencv_highgui>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)
