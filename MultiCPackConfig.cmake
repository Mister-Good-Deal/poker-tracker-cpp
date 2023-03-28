if (${CPACK_WINDOWS})
    include("build-windows-release/CPackConfig.cmake")

    set(CPACK_INSTALL_CMAKE_PROJECTS
        "build-windows-debug;poker-bot;ALL;/"
        "build-windows-release;poker-bot;ALL;/"
        )
elseif (${CPACK_LINUX})
    include("build-linux-release/CPackConfig.cmake")

    set(CPACK_INSTALL_CMAKE_PROJECTS
        "build-linux-debug;poker-bot;ALL;/"
        "build-linux-release;poker-bot;ALL;/"
        )
endif ()