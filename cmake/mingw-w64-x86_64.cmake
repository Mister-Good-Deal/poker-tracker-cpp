set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)
# Set mingw compiler
set(CMAKE_C_COMPILER /usr/bin/${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_RC_COMPILER /usr/bin/${TOOLCHAIN_PREFIX}-windres)
# target environment on the build host system
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
# modify default behavior of FIND_XXX() commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# Export all symbols in DLL
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
