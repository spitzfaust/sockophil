project(sockophil)

set(SOURCE_FILES
        Validator.cpp ActionPackage.cpp FileInfoPackage.cpp Helper.cpp FileInfoPackage.cpp ListPackage.cpp Networking.cpp ErrorPackage.cpp Package.cpp SuccessPackage.cpp ErrnoExceptions.cpp LoginPackage.cpp AccessPackage.cpp)
add_library(sockophil_lib STATIC ${SOURCE_FILES})