project(sockclient)
set(SOURCE_FILES
        Client.cpp Menu.cpp NoFilenameException.cpp)
add_library(sockclient_lib STATIC ${SOURCE_FILES})
target_link_libraries(sockclient_lib sockophil_lib)
add_executable(sockclient sockclient.cpp)
target_link_libraries(sockclient docopt_s sockclient_lib)