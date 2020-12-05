cmake_minimum_required(VERSION 2.4)
project(html-parser-and-extractor)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(PROJECT_CODE
    src/main.cpp
    src/sources.cpp)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

include_directories(include ${CURL_INCLUDE_DIR})

add_executable(run ${PROJECT_CODE})

target_link_libraries(run ${CURL_LIBRARIES})
