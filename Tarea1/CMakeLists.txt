cmake_minimum_required(VERSION 3.8)

# Project Info
project(Tarea1)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Headers & Sources
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/Point.cpp
    ${PROJECT_SOURCE_DIR}/src/Entry.cpp
    ${PROJECT_SOURCE_DIR}/src/Node.cpp
    # ${PROJECT_SOURCE_DIR}/src/MTreeCP.cpp
)

# Main executable
# add_executable(
#     cp_main
#     ${PROJECT_SOURCE_DIR}/src/MTreeCP.cpp
#     ${SOURCES}
# )


# Test settings
enable_testing()

add_executable(
  cp_tests
  ${PROJECT_SOURCE_DIR}/tests/cp_tests.cc
  ${SOURCES}
)
target_link_libraries(
  cp_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cp_tests)