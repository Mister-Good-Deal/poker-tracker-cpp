cmake_minimum_required(VERSION 3.25)
project(logger)

# Make cache variables for install destinations
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required libs
find_package(fmt REQUIRED)
find_package(quill REQUIRED)

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

target_link_libraries(logger PRIVATE fmt::fmt quill::quill)

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
