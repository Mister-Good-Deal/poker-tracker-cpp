find_package(GTest REQUIRED)

function(add_class_test class_name test_file)
    set(targetName "${class_name}Test")

    add_executable(${targetName} ${test_file})

    target_link_libraries(${targetName} poker_game gtest_main GTest::gtest GTest::gmock)
    target_include_directories(${targetName} PRIVATE ../include)
    gtest_add_tests(TARGET ${targetName} SOURCES ${test_file} TEST_LIST testsList)
endfunction()

add_class_test(Card CardTest.cpp)
add_class_test(CardFactory CardFactoryTest.cpp)
add_class_test(Player PlayerTest.cpp)
add_class_test(Round RoundTest.cpp)

add_executable(poker_game_tests main.cpp)
target_link_libraries(poker_game_tests poker_game GTest::gtest GTest::gmock)
