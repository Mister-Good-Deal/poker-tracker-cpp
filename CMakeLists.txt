cmake_minimum_required(VERSION 3.25)

if (BUILD_TESTS)
    enable_testing()
endif ()

# Check the system type
if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    message(STATUS "Windows build")

    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
    set(CMAKE_GENERATOR "MinGW Makefiles")
else ()
    message(STATUS "Linux build")

    set(CMAKE_GENERATOR "Unix Makefiles")
endif ()

# Configure the logger
if (NOT DEFINED QUILL_ACTIVE_LOG_LEVEL)
    message(STATUS "QUILL_ACTIVE_LOG_LEVEL is not set, getting default value based on CMAKE_BUILD_TYPE")

    string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)

    if (${build_type} STREQUAL debug)
        message(STATUS "LOG_LEVEL is set to DEBUG")

        add_compile_definitions(QUILL_ACTIVE_LOG_LEVEL=QUILL_LOG_LEVEL_DEBUG)
    else ()
        message(STATUS "LOG_LEVEL is set to INFO")

        add_compile_definitions(QUILL_ACTIVE_LOG_LEVEL=QUILL_LOG_LEVEL_INFO)
    endif ()
endif ()

project(poker_bot)

add_subdirectory(services/logger)
add_subdirectory(services/game_handler)
add_subdirectory(services/ocr)
add_subdirectory(services/scraper)
