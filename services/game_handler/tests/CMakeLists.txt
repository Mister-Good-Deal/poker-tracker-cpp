find_package(GTest REQUIRED)

function(add_class_test class_name)
    set(target_name "${class_name}Test")
    set(test_src_file "${target_name}.cpp")

    add_executable(${target_name} ${test_src_file})

    target_link_libraries(${target_name} game_handler GTest::Main GTest::gtest GTest::gmock)
    target_include_directories(${target_name} PRIVATE ../include)
    gtest_add_tests(TARGET ${target_name} SOURCES ${test_src_file})
endfunction()

message(NOTICE "The unit tests for game_handler are stored in ${CMAKE_BINARY_DIR}")

add_class_test(Board)
add_class_test(Card)
add_class_test(CardFactory)
add_class_test(Game)
add_class_test(Hand)
add_class_test(Player)
add_class_test(Round)
add_class_test(RoundAction)
