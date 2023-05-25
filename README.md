# Poker bot

Poker bot program written in C++ to play in online poker rooms like Winamax or Unibet on no-limit Texas Holdem 3
players short stack style (Spin and Go).

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
- [quill](https://github.com/odygrd/quill) - Quill is a cross-platform low latency logging library based on C++14/C++17.
- [uwebsockets](https://github.com/uNetworking/uWebSockets) - Simple, secure & standards compliant web server for the most
  demanding of applications
- [GoogleTest](https://github.com/google/googletest) - Testing library for runnings services unit tests

## Services

The bot is composed of multiple services that work together to provide its functionality.

### Logger

This service is responsible for handling logging functionalities. It provides logging capabilities for the entire bot.

*Services used*

- None

*Libraries used*

- quill

### Game Handler

This service represents the game status and history. It logs all the game's events and can serialize them in JSON format.

*Services used*

- Logger

*Libraries used*

- nlohmann-json

### Websockets

This service handles communication with the poker room via websockets. It enables real-time data exchange between the bot and the poker
room.

*Services used*

- Logger

*Libraries used*

- uwebsockets

### OCR

This service reads inputs from a poker room screenshot and extracts all the necessary information from it using optical character
recognition (OCR) techniques.

*Services used*

- Logger
- Game Handler

*Libraries used*

- OpenCV

### Scrapper

This service scrapes data from the poker room's user interface to gather information about ongoing games, players, and other relevant data.

*Services used*

- Logger
- Game Handler

*Libraries used*

- OpenCV
- X11
- Xext

### Game Session

This service manages the game session and handles game-related functionalities such as starting new games, managing player actions, and
determining game outcomes.

*Services used*

- Logger
- Game Handler

*Libraries used*

- nlohmann-json

### Dependencies graph

Here the dependencies graph of the services with libraries.

```plantuml
!define AWSPUML https://raw.githubusercontent.com/awslabs/aws-icons-for-plantuml/v16.0/dist

package "Logger Service" {
    component Logger
    folder "Libraries (Logger)" {
        component Quill_Logger
    }
}

package "Game Handler Service" {
    component GameHandler
    folder "Libraries (Game Handler)" {
        component NlohmannJson_GameHandler
    }
}

package "Websockets Service" {
    component Websockets
    folder "Libraries (Websockets)" {
        component UWebsockets_Websockets
    } 
}

package "OCR Service" {
    component OCR
    folder "Libraries (OCR)" {
        component OpenCV_OCR
        component opencv_core_OCR
        component opencv_imgproc_OCR
        component opencv_text_OCR
    }
}

package "Scrapper Service" {
    component Scrapper
    folder "Libraries (Scrapper)" {
        component OpenCV_Scrapper
        component opencv_core_Scrapper
        component opencv_imgproc_Scrapper
        component opencv_imgcodecs_Scrapper
        component X11_Scrapper
        component Xext_Scrapper
    }
}

package "Game Session Service" {
    component GameSession
    folder "Libraries (Game Session)" {
        component NlohmannJson_GameSession
    }
}

Logger --> GameHandler
Logger --> Websockets
Logger --> OCR
GameHandler --> OCR
GameHandler --> Scrapper
GameHandler --> GameSession
```

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
