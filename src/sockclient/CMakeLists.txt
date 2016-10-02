project(sockclient)
set(SOURCE_FILES
        client.cpp current_directory_exception.cpp menu.cpp client_selection.cpp socket_creation_exception.cpp socket_connection_exception.cpp)
add_library(sockclient_lib ${SOURCE_FILES})
add_executable(sockclient sockclient.cpp)
target_link_libraries(sockclient docopt_s sockclient_lib)