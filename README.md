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

## Coding

### Git hook

A client side [git hook](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) can be setup to automatically check that your code
is correctly formatted before committing.

The actual setup hook is a `pre-commit` hook located in `hooks/pre-commit` that verify that C++ files are
correctly formatted and correct the files if possible.

To enable this hook run `init.sh` at the root project directory.

To bypass this hook you can commit with the following option `git commit --no-verify`.

### Apply on all the project source files

To apply the code formatting on all project source files at start, use the following command :

```bash
find services -regex '.*\.\(cpp\|hpp\)' -exec clang-format -style=file -i {} \;
```
