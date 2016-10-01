include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runServerTests
            basic_check.cpp
        )

target_link_libraries(runServerTests gtest gtest_main)
