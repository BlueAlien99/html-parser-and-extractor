cmake_minimum_required(VERSION 2.4)
project(html-parser-and-extractor)
set(CMAKE_CXX_STANDARD 17)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fprofile-arcs -ftest-coverage -fPIC -O0")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -fPIC -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CURL_LIBRARY "-lcurl -licuuc")
find_package(CURL REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(include ${CURL_INCLUDE_DIR})

add_library(project-lib 
    src/sources.cpp
    src/lexer.cpp
    src/html_parser.cpp
    src/conf_parser.cpp
    src/node.cpp
    src/conf_object.cpp
    src/utils.cpp
    src/extractor.cpp
    src/node_iterator.cpp)

target_link_libraries(project-lib ${CURL_LIBRARIES})

add_executable(run src/main.cpp)
add_executable(test tests/main.cpp
    tests/sources.cpp
    tests/lexer_html.cpp
    tests/lexer_conf.cpp
    tests/html_parser.cpp
    tests/conf_parser.cpp
    tests/extractor.cpp
    tests/node_iterator.cpp)

target_link_libraries(run project-lib)
target_link_libraries(test project-lib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_MODULE_PATH ./CMakeModules)
    if(CMAKE_COMPILER_IS_GNUCXX)
        include(CodeCoverage)
        setup_target_for_coverage_gcovr_html(
            NAME coverage
            EXECUTABLE ./bin/test)
    endif()
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
