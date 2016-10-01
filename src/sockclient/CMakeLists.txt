project(sockclient)
set(SOURCE_FILES
        sockclient.cpp)

add_executable(sockclient ${SOURCE_FILES})
target_link_libraries(sockclient docopt_s)