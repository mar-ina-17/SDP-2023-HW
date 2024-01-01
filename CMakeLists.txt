cmake_minimum_required(VERSION 3.10)

project(TennisProject_82091)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Use FetchContent to include Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.4  # You can specify the version you want
)
FetchContent_MakeAvailable(Catch2)

# Set source files
set(SOURCES
    src/hw.cpp
    src/headers/cmd.h
    src/headers/node.h
    src/headers/tree.h
)

# Add the main executable
add_executable(program_build ${SOURCES})

# Include directories for main executable
target_include_directories(program_build PRIVATE src/headers)

# Set test files
set(TESTS1
    tests/main.cpp
    tests/node_tests.cpp
)
set(TESTS2
    tests/main.cpp
    tests/tree_tests.cpp
)
# set(TESTS3
#     tests/main.cpp
#     tests/cmd_tests.cpp
# )
# Add the tests executables
add_executable(node_test_build ${TESTS1})
add_executable(tree_test_build ${TESTS2})
# add_executable(cmd_test_build ${TESTS3})

# Include directories for tests executables
target_include_directories(node_test_build PRIVATE src/headers tests)
target_include_directories(tree_test_build PRIVATE src/headers tests)
# target_include_directories(cmd_test_build PRIVATE src/headers tests)

# Link the tests executables with Catch2
target_link_libraries(node_test_build PRIVATE Catch2::Catch2)
target_link_libraries(tree_test_build PRIVATE Catch2::Catch2)
# target_link_libraries(cmd_test_build PRIVATE Catch2::Catch2)

# Enable testing
include(CTest)
add_test(NAME node_test_build COMMAND node_test_build)
add_test(NAME tree_test_build COMMAND tree_test_build)
# add_test(NAME cmd_test_build COMMAND cmd_test_build)
