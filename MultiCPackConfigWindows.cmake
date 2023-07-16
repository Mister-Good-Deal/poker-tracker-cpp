
    include("build-windows-release/CPackConfig.cmake")

    set(CPACK_INSTALL_CMAKE_PROJECTS
        "build-windows-debug;poker-bot;ALL;/"
        "build-windows-release;poker-bot;ALL;/"
        )
