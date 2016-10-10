include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runClientTests
            basic_check.cpp
        menu_test.cpp client_selection_test.cpp)

target_link_libraries(runClientTests gtest gtest_main sockclient_lib)