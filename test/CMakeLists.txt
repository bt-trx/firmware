cmake_minimum_required(VERSION 2.8.8)
project(arduino-mock-test-all)

message("building all tests")

find_package(Threads REQUIRED)
add_subdirectory(arduino_mock)

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/googletest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/googlemock/include
)

file(GLOB LIB_SRCS "lib/*.cpp")
file(GLOB TEST_SRCS "*.cpp")
file(GLOB PROGRAM_SRCS "../src/bttrx_fsm.cpp" "../src/splitstring.cpp" "../src/wt32i.cpp" "../src/serialwrapper.cpp" "../src/led.cpp" "../src/button.cpp")
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
