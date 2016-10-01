include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runClientTests
            basic_check.cpp
        )

target_link_libraries(runClientTests gtest gtest_main)