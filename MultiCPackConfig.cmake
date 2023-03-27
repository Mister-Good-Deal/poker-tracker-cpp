include("release/CPackConfig.cmake")

if (WIN32)
    set(CPACK_INSTALL_CMAKE_PROJECTS
        "build-linux-debug;poker-bot;ALL;/"
        "build-linux-release;poker-bot;ALL;/"
        )
elseif (LINUX)
    set(CPACK_INSTALL_CMAKE_PROJECTS
        "build-windows-debug;poker-bot;ALL;/"
        "build-windows-release;poker-bot;ALL;/"
        )
endif ()