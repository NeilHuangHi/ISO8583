cmake_minimum_required(VERSION 3.15)
project(ISO8583 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only library: consumers only need the include path.
add_library(iso8583 INTERFACE)
target_include_directories(iso8583 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Smoke test driver.
add_executable(iso8583_demo main.cpp)
target_link_libraries(iso8583_demo PRIVATE iso8583)

if(MSVC)
    target_compile_options(iso8583_demo PRIVATE /W3 /utf-8)
else()
    target_compile_options(iso8583_demo PRIVATE -Wall)
endif()

enable_testing()

add_executable(round_trip_test tests/round_trip_test.cpp)
target_link_libraries(round_trip_test PRIVATE iso8583)
if(MSVC)
    target_compile_options(round_trip_test PRIVATE /W3 /utf-8)
else()
    target_compile_options(round_trip_test PRIVATE -Wall)
endif()

add_test(NAME round_trip COMMAND round_trip_test)

add_executable(build_message_test tests/build_message_test.cpp)
target_link_libraries(build_message_test PRIVATE iso8583)
if(MSVC)
    target_compile_options(build_message_test PRIVATE /W3 /utf-8)
else()
    target_compile_options(build_message_test PRIVATE -Wall)
endif()

add_test(NAME build_message COMMAND build_message_test)
