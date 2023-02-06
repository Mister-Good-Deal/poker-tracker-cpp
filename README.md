# Poker bot

Poker bot program written in C++ to play in online poker rooms like Winamax or Unibet on no-limit Texas Holdem 3
players short stack style (Expresso).

## Compilation

- [C++20](https://en.cppreference.com/w/cpp/20) standard
- [CMake](https://cmake.org/) building tool
- Handle [GCC](https://gcc.gnu.org/), [CLang](https://clang.llvm.org/) and [MinGW](https://www.mingw-w64.org/) compilers
- Cross compilation for Linux with GCC/CLang and Windows with MinGW

## Libraries

- [OpenCV](https://github.com/opencv/opencv) - Computer vision library to read inputs via OCR on a given image
- [SDL](https://github.com/libsdl-org/SDL) - Cross-platform development library to provide low level access to keyboard,
  mouse and graphics
- [nlohmann-json](https://github.com/nlohmann/json) - Allow to use JSON representation of a game for archived and later
  reprocessing
- [GoogleTest](https://github.com/google/googletest) - Testing library for runnings services unit tests

## Services

The bot is composed of multiple services which are working together.

### Game handler

This service represents the game status and historic.

It logs all the game's events and can serialize them in JSON format.

*Libraries used*

- nlohmann-json

### OCR

This service reads inputs from a poker room screenshot to extract all needed information from it.

*Libraries used*

- OpenCV

### todo