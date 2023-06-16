cmake_minimum_required(VERSION 3.24)

#-----------------------------------------------------------------------------------------------------------------------
# Resolve version
#-----------------------------------------------------------------------------------------------------------------------

include(cmake/helpers.cmake)

extract_version()

project(poker_bot VERSION ${POKER_BOT_VERSION} LANGUAGES CXX)

message(STATUS "POKER_BOT_VERSION ${POKER_BOT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)

#-----------------------------------------------------------------------------------------------------------------------
# Configure the logger
#-----------------------------------------------------------------------------------------------------------------------

if (NOT DEFINED QUILL_ACTIVE_LOG_LEVEL)
    message(STATUS "QUILL_ACTIVE_LOG_LEVEL is not set, getting default value based on CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")

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
# Configure the debug mode
#-----------------------------------------------------------------------------------------------------------------------

if (${build_type} STREQUAL debug)
    message(STATUS "DISPLAY_OPENCV_IMG is set to 1 in DEBUG mode")

    add_compile_definitions(DISPLAY_OPENCV_IMG=1)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Add subproject modules / services
#-----------------------------------------------------------------------------------------------------------------------

add_subdirectory(services/logger)
add_subdirectory(services/game_handler)
add_subdirectory(services/ocr)
add_subdirectory(services/scraper)
add_subdirectory(services/websockets)
add_subdirectory(services/game_session)

#-----------------------------------------------------------------------------------------------------------------------
# Add and link the program
#-----------------------------------------------------------------------------------------------------------------------

add_executable(poker_bot main.cpp)

set_target_properties(poker_bot PROPERTIES DEBUG_POSTFIX _debug)

target_link_libraries(poker_bot logger game_handler ocr scraper websockets)

#-----------------------------------------------------------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------------------------------------------------------

install(
        TARGETS poker_bot
        EXPORT poker_bot_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        EXPORT poker_bot_targets
        FILE poker_bot_targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
)

if (WIN32)
    # Add necessary DLLs
    install(
            FILES /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll /usr/x86_64-w64-mingw32/bin/libstdc++-6.dll
            /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll /usr/x86_64-w64-mingw32/bin/libfmt.dll
            DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Packaging
#-----------------------------------------------------------------------------------------------------------------------

if (WIN32)
    set(CPACK_GENERATOR ZIP)

    include(InstallRequiredSystemLibraries)
elseif (LINUX)
    set(CPACK_GENERATOR TGZ)
endif ()

set(CPACK_PACKAGE_NAME "poker-bot")
set(CPACK_PACKAGE_VENDOR "Romain Laneuville")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ poker bot for playing on online rooms")
set(CPACK_PACKAGE_DIRECTORY ${PROJECT_BINARY_DIR})

include(CPack)
