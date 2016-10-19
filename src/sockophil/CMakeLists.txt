project(sockophil)

set(SOURCE_FILES
        ErrnoException.cpp Validator.cpp RequestPackage.cpp DataPackage.cpp SocketCreationException.cpp Helper.cpp)
add_library(sockophil_lib STATIC ${SOURCE_FILES})