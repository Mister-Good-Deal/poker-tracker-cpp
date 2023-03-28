include("build-linux-release/CPackConfig.cmake")

set(CPACK_INSTALL_CMAKE_PROJECTS
    "build-linux-debug;poker-bot;ALL;/"
    "build-linux-release;poker-bot;ALL;/"
    )
