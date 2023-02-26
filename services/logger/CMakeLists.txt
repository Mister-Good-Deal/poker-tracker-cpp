cmake_minimum_required(VERSION 3.25)
project(logger LANGUAGES CXX)

# Make cache variables for install destinations
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required libs
find_package(quill CONFIG REQUIRED)

# Source and header files
set(
    SRC
    src/Logger.cpp
)

set(
    HEADERS
    include/Logger.hpp
)

# Create the library
add_library(logger STATIC ${SRC})

target_include_directories(
    logger
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(logger PRIVATE quill::quill)

install(
    TARGETS logger
    EXPORT logger_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT logger_targets
    FILE logger_targets.cmake
    NAMESPACE Logger::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger
)
