project(sockserver)
set(SOURCE_FILES
        sockserver.cpp)

add_executable(sockserver ${SOURCE_FILES})
target_link_libraries(sockserver docopt_s)