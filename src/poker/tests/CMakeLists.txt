find_package(GTest REQUIRED)

function(add_class_test class_name test_file)
    set(targetName "${class_name}Test")

    add_executable(${targetName} ${test_file})

    target_link_libraries(${targetName} poker_game GTest::gtest GTest::gmock)
    target_include_directories(${targetName} PRIVATE ${PROJECT_SOURCE_DIR}/tests)
    gtest_add_tests(TARGET ${targetName} SOURCES ${test_file} TEST_LIST testsList)
endfunction()

add_class_test(card CardTest.cpp)
add_class_test(player PlayerTest.cpp)
add_class_test(round RoundTest.cpp)
