cmake_minimum_required(VERSION 3.25)

if (BUILD_TESTS)
    enable_testing()
endif ()

# Check the system type
if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
    set(CMAKE_GENERATOR "MinGW Makefiles")
else ()
    # Default to native build
    set(CMAKE_GENERATOR "Unix Makefiles")
endif ()

if (${LOG_LEVEL})
    message(STATUS "LOG_LEVEL = ${LOG_LEVEL}")

    add_definitions(-DLOG_LEVEL="${LOG_LEVEL}")
else ()
    string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)

    if (${build_type} STREQUAL debug)
        message(NOTICE "LOG_LEVEL is not defined, set default to DEBUG")

        add_definitions(-DLOG_LEVEL_DEBUG=1)
    else ()
        message(NOTICE "LOG_LEVEL is not defined, set default to INFO")

        add_definitions(-DLOG_LEVEL_INFO=1)
    endif ()
endif ()

project(poker_bot)

add_subdirectory(services/logger)
add_subdirectory(services/game_handler)
add_subdirectory(services/ocr)
