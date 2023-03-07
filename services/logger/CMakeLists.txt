cmake_minimum_required(VERSION 3.24)

project(logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-----------------------------------------------------------------------------------------------------------------------
# Libs dependencies
#-----------------------------------------------------------------------------------------------------------------------

find_package(quill CONFIG REQUIRED)

#-----------------------------------------------------------------------------------------------------------------------
# Source files definition
#-----------------------------------------------------------------------------------------------------------------------

set(
    SRC
    src/Logger.cpp
)

set(
    HEADERS
    include/Logger.hpp
)

#-----------------------------------------------------------------------------------------------------------------------
# Library target creation and setup
#-----------------------------------------------------------------------------------------------------------------------

add_library(logger SHARED ${SRC})

target_include_directories(
    logger
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(logger PUBLIC quill::quill)

#-----------------------------------------------------------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------------------------------------------------------

include(GNUInstallDirs)

install(
    TARGETS logger
    EXPORT logger_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logger
)

install(
    EXPORT logger_targets
    FILE logger_targets.cmake
    NAMESPACE Logger::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger
)

# Quill lib install dependencies

install(
    FILES $<TARGET_FILE:quill::quill>
    DESTINATION $<IF:$<STREQUAL:$<TARGET_FILE_SUFFIX:quill::quill>,.dll>,${CMAKE_INSTALL_BINDIR},${CMAKE_INSTALL_LIBDIR}>
)
