include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runLibTests
            basic_check.cpp
            helper_tests.cpp
        )

target_link_libraries(runLibTests gtest gtest_main sockophil_lib)
