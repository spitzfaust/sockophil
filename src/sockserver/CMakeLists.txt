project(sockserver)
set(SOURCE_FILES
        server.cpp sockserver.cpp)

add_executable(sockserver ${SOURCE_FILES})
target_link_libraries(sockserver docopt_s)