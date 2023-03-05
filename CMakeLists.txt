cmake_minimum_required(VERSION 3.24)

project(poker_bot)

#-----------------------------------------------------------------------------------------------------------------------
# Enable tests compilation on demand
#-----------------------------------------------------------------------------------------------------------------------

if (BUILD_TESTS)
    enable_testing()
    # GoogleTest is linked as shared lib
    add_compile_definitions(GTEST_LINKED_AS_SHARED_LIBRARY=1)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Set default build to release
#-----------------------------------------------------------------------------------------------------------------------

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Configure the logger
#-----------------------------------------------------------------------------------------------------------------------

if (NOT DEFINED QUILL_ACTIVE_LOG_LEVEL)
    message(STATUS "QUILL_ACTIVE_LOG_LEVEL is not set, getting default value based on CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")

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

#-----------------------------------------------------------------------------------------------------------------------
# Add subproject modules / services
#-----------------------------------------------------------------------------------------------------------------------

add_subdirectory(services/logger)
add_subdirectory(services/game_handler)
add_subdirectory(services/ocr)
add_subdirectory(services/scraper)

#-----------------------------------------------------------------------------------------------------------------------
# Add and link the program
#-----------------------------------------------------------------------------------------------------------------------

add_executable(main main.cpp)

target_link_libraries(main logger)

#-----------------------------------------------------------------------------------------------------------------------
# Packaging
#-----------------------------------------------------------------------------------------------------------------------

if (WIN32)
    set(CPACK_GENERATOR ZIP)
elseif (LINUX)
    set(CPACK_GENERATOR TGZ)
endif ()

set(CPACK_PACKAGE_NAME "PokerBot")
set(CPACK_PACKAGE_VENDOR "Romain Laneuville")
set(CPACK_PACKAGE_DIRECTORY ${PROJECT_BINARY_DIR})

include(CPack)
