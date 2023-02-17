cmake_minimum_required(VERSION 3.25)
project(ocr)

# Make cache variables for install destinations
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required libs
# logger and GameHandler targets are automatically imported by parent project call
find_package(quill CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

# Source and header files
set(
    SRC
    src/OcrInterface.cpp
    src/WinamaxOcr.cpp
)

set(
    HEADERS
    include/OcrInterface.hpp
    include/WinamaxOcr.hpp
)

# Create the library
add_library(ocr STATIC ${SRC})

target_include_directories(
    ocr
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
    logger
    game_handler
)

target_link_libraries(ocr PRIVATE logger game_handler opencv_core opencv_imgproc opencv_text)

install(
    TARGETS ocr
    EXPORT ocr_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT ocr_targets
    FILE ocr_targets.cmake
    NAMESPACE OCR::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ocr
)

# To use the exported ocr library in other projects use:
# include(${INSTALL_PREFIX}/lib/cmake/ocr_targets.cmake)
# ...
# target_link_libraries(someTarget PRIVATE ocr::ocr)

# Add the test subdirectory for building tests
if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()