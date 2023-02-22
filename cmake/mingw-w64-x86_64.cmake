message(STATUS "Windows build")
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)
# Set mingw compiler
set(CMAKE_C_COMPILER /usr/local/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /usr/local/bin/x86_64-w64-mingw32-g++)
# target environment on the build host system
set(CMAKE_FIND_ROOT_PATH /usr/mingw)
# modify default behavior of FIND_XXX() commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
