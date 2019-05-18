cmake_minimum_required(VERSION 2.8.8)
project(arduino-mock-test-all)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "Set the C++ standard to be used for compiling")

message("building all tests")

find_package(Threads REQUIRED)
add_subdirectory(arduino_mock)

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/googletest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/googlemock/include
)

file(GLOB LIB_SRCS ${PROJECT_SOURCE_DIR}/lib/*.cpp)
file(GLOB TEST_SRCS ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB PROGRAM_SRCS ${PROJECT_SOURCE_DIR}/../src/*.cpp)
list(REMOVE_ITEM PROGRAM_SRCS ${PROJECT_SOURCE_DIR}/../src/main.cpp)

add_executable(test-all ${TEST_SRCS} ${PROGRAM_SRCS} ${LIB_SRCS})

target_link_libraries(test-all
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/googlemock/gtest/libgtest.a
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/googlemock/libgmock.a
    ${ARDUINO_MOCK_LIBS_DIR}/dist/lib/libarduino_mock.a
    ${CMAKE_THREAD_LIBS_INIT}
)

add_dependencies(test-all arduino_mock)

enable_testing()
add_test(TestAll test-all)
