find_package(GTest REQUIRED)

function(add_class_test class_name)
    set(target_name "${class_name}Test")
    set(test_src_file "${target_name}.cpp")

    add_executable(${target_name} ${test_src_file})

    target_link_libraries(${target_name} ocr logger game_handler opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_text GTest::Main GTest::gtest GTest::gmock)
    target_include_directories(${target_name} PRIVATE ../include game_handler)
    gtest_add_tests(TARGET ${target_name} SOURCES ${test_src_file})
endfunction()

# Add the image file as a resource
set(SUIT_IMAGES_DIR "${PROJECT_SOURCE_DIR}/resources/winamax/suits")
set(RANK_IMAGES_DIR "${PROJECT_SOURCE_DIR}/resources/winamax/ranks")

add_definitions(-DSUIT_IMAGES_DIR="${SUIT_IMAGES_DIR}")
add_definitions(-DRANK_IMAGES_DIR="${RANK_IMAGES_DIR}")

message(STATUS "The unit tests for ocr are stored in ${CMAKE_BINARY_DIR}")

add_class_test(WinamaxOcr)
