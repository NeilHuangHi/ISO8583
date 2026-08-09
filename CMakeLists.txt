cmake_minimum_required(VERSION 3.15)
project(ISO8583 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only library: consumers only need the include path.
add_library(iso8583 INTERFACE)
target_include_directories(iso8583 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

function(add_iso8583_executable name src)
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE iso8583)
    if(MSVC)
        target_compile_options(${name} PRIVATE /W3 /utf-8)
    else()
        target_compile_options(${name} PRIVATE -Wall)
    endif()
endfunction()

# Smoke test driver.
add_iso8583_executable(iso8583_demo main.cpp)

enable_testing()

add_iso8583_executable(round_trip_test tests/round_trip_test.cpp)
add_test(NAME round_trip COMMAND round_trip_test)

add_iso8583_executable(build_message_test tests/build_message_test.cpp)
add_test(NAME build_message COMMAND build_message_test)
