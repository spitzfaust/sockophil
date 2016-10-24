# sockophil

## How to build
You need to have CMake installed to build the project.

1. Create a build folder `mkdir build`
2. Create a bin folder `mkdir bin`
3. Change into the build folder `cd build`
4. Run CMake `cmake ..`
5. While in the build folder run Make `make`
6. After the compiling the executables are placed in the `bin` folder

## src

The `src` folder holds the **sockclient** and **sockserver** implementations.

## include

The `include` folder holds header-files of the **sockclient** and **sockserver** in seperate subfolders.
Third party header-files are also put in this directory.

## lib

The `lib` folder holds third party and project libraries.

## test

The test folder holds the client and server tests.
For the testing [Google Test](https://github.com/google/googletest) is used.

### test/lib

The `test/lib` folder holds the gtest src files.

## bin

The `bin` folder holds compiled executables.
```
.
├── runClientTests
├── runLibTests
├── runServerTests
├── sockclient
└── sockserver
```
