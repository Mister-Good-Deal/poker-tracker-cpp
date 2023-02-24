cmake_minimum_required(VERSION 3.25)

if (BUILD_TESTS)
    enable_testing()
endif ()

project(poker_bot)

# Configure the logger
if (NOT DEFINED QUILL_ACTIVE_LOG_LEVEL)
    message(STATUS "QUILL_ACTIVE_LOG_LEVEL is not set, getting default value based on CMAKE_BUILD_TYPE")

    string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)

    if (${build_type} STREQUAL debug)
        message(STATUS "LOG_LEVEL is set to QUILL_LOG_LEVEL_DEBUG")

        add_compile_definitions(QUILL_ACTIVE_LOG_LEVEL=QUILL_LOG_LEVEL_DEBUG)
    else ()
        message(STATUS "LOG_LEVEL is set to QUILL_LOG_LEVEL_INFO")

        add_compile_definitions(QUILL_ACTIVE_LOG_LEVEL=QUILL_LOG_LEVEL_INFO)
    endif ()
else ()
    message(STATUS "LOG_LEVEL is set to ${QUILL_ACTIVE_LOG_LEVEL}")

    add_compile_definitions(QUILL_ACTIVE_LOG_LEVEL=${QUILL_ACTIVE_LOG_LEVEL})
endif ()

add_subdirectory(services/logger)
add_subdirectory(services/game_handler)
add_subdirectory(services/ocr)
add_subdirectory(services/scraper)

add_executable(main main.cpp)
target_link_libraries(main logger)

