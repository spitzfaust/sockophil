project(sockophil)

set(SOURCE_FILES
        Validator.cpp ActionPackage.cpp DataPackage.cpp Helper.cpp ListPackage.cpp Networking.cpp ErrorPackage.cpp Package.cpp SuccessPackage.cpp ErrnoExceptions.cpp)
add_library(sockophil_lib STATIC ${SOURCE_FILES})