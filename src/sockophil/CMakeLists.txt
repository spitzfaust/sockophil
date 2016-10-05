project(sockophil)

set(SOURCE_FILES
        ErrnoException.cpp Validator.cpp)
add_library(sockophil_lib STATIC ${SOURCE_FILES})