cmake_minimum_required(VERSION 3.24)

project(websockets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-----------------------------------------------------------------------------------------------------------------------
# Libs dependencies
#-----------------------------------------------------------------------------------------------------------------------

find_package(uwebsockets CONFIG REQUIRED)

#-----------------------------------------------------------------------------------------------------------------------
# Source files definition
#-----------------------------------------------------------------------------------------------------------------------

set(
    SRC
    src/Server.cpp
)

#-----------------------------------------------------------------------------------------------------------------------
# Library target creation and setup
#-----------------------------------------------------------------------------------------------------------------------

add_library(websockets SHARED ${SRC})

target_include_directories(
    websockets
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
    uws::uws
)

target_link_libraries(websockets PUBLIC logger uws::uws)

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
    TARGETS websockets
    EXPORT websockets_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/websockets
)

install(
    EXPORT websockets_targets
    FILE websockets_targets.cmake
    NAMESPACE Websockets::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/websockets
)

if (WIN32)
    install(
        FILES $<TARGET_FILE:uws::uws> /usr/x86_64-w64-mingw32/bin/libzlib1.dll /usr/x86_64-w64-mingw32/bin/libuv.dll
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
elseif (LINUX)
    install(
        FILES $<TARGET_FILE:uws::uws>
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif ()
