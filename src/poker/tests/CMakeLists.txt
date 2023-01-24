function(add_class_test class_name test_file)
    add_executable(${class_name}Test ${test_file})
    target_link_libraries(${class_name}Test
            poker_game
            CONAN_PKG::gtest
            )
    target_include_directories(${class_name}Test
            PRIVATE
            ${PROJECT_SOURCE_DIR}/test
            )
    add_test(NAME ${class_name}Test COMMAND ${class_name}Test)
endfunction()

add_class_test(card CardTest.cpp)
add_class_test(player PlayerTest.cpp)
add_class_test(round RoundTest.cpp)
