find_package(GTest REQUIRED)

function(add_class_test class_name)
    set(target_name "${class_name}Test")
    set(test_src_file "${target_name}.cpp")

    add_executable(${target_name} ${test_src_file})

    target_link_libraries(${target_name} scraper logger game_handler opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs GTest::Main GTest::gtest GTest::gmock)
    target_include_directories(${target_name} PRIVATE ../include game_handler)
    gtest_add_tests(TARGET ${target_name} SOURCES ${test_src_file})

    include(GNUInstallDirs)

    install(
            TARGETS ${target_name}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/scraper
    )
endfunction()

add_class_test(WinamaxScraper)
