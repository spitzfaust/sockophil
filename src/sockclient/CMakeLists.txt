project(sockclient)
set(SOURCE_FILES
        client.cpp current_directory_exception.cpp menu.cpp)
add_library(sockclient_lib STATIC ${SOURCE_FILES})
add_executable(sockclient sockclient.cpp)
target_link_libraries(sockclient docopt_s sockclient_lib)