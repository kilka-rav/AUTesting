cmake_minimum_required(VERSION 3.28.1)
project(AUTesting)

set(EXAMPLES ON)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin")

include(FetchContent)
FetchContent_Declare(
  googletest
URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)

if(EXAMPLES)
    add_subdirectory(examples)
endif()