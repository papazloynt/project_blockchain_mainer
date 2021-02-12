cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

set(HUNTER_CONFIGURATION_TYPES Release)

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
  SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
  FILEPATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake"
)

project(blockchain)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument -Wno-unused-parameter")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

hunter_add_package(gRPC)
find_package(gRPC CONFIG REQUIRED)

hunter_add_package(Protobuf)
find_package(Protobuf CONFIG REQUIRED)

# For SQLite3
find_package(Threads)

add_subdirectory("third-party/")

set(API_GENERATED_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/ccpkg)

set(PROTO_GENERATED_SOURCES
        "${API_GENERATED_DIR}/blockchain.pb.cc"
        "${API_GENERATED_DIR}/google/api/annotations.pb.cc"
        "${API_GENERATED_DIR}/google/api/http.pb.cc"
        "${API_GENERATED_DIR}/google/rpc/status.pb.cc")

set(PROTO_GENERATED_HEADERS
        "${API_GENERATED_DIR}/blockchain.pb.h"
        "${API_GENERATED_DIR}/google/api/annotations.pb.h"
        "${API_GENERATED_DIR}/google/api/http.pb.h"
        "${API_GENERATED_DIR}/google/rpc/status.pb.h")

set(PROTO_GENERATED_GRPC_SOURCES
        "${API_GENERATED_DIR}/blockchain.grpc.pb.cc"
        "${API_GENERATED_DIR}/google/api/annotations.grpc.pb.cc"
        "${API_GENERATED_DIR}/google/api/http.grpc.pb.cc"
        "${API_GENERATED_DIR}/google/rpc/status.grpc.pb.cc")

set(PROTO_GENERATED_GRPC_HEADERS
        "${API_GENERATED_DIR}/blockchain.grpc.pb.h"
        "${API_GENERATED_DIR}/google/api/annotations.grpc.pb.h"
        "${API_GENERATED_DIR}/google/api/http.grpc.pb.h"
        "${API_GENERATED_DIR}/google/rpc/status.grpc.pb.h")

find_program(PROTOBUF_PROTOC protoc)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

set(PROTO_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/protos/blockchain.proto"
        "${CMAKE_CURRENT_SOURCE_DIR}/protos/google/api/annotations.proto"
        "${CMAKE_CURRENT_SOURCE_DIR}/protos/google/api/http.proto"
        "${CMAKE_CURRENT_SOURCE_DIR}/protos/google/rpc/status.proto")

set(PROTO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/protos")

add_custom_command(
        OUTPUT  ${PROTO_GENERATED_SOURCES}
        ${PROTO_GENERATED_HEADERS}
        ${PROTO_GENERATED_GRPC_SOURCES}
        ${PROTO_GENERATED_GRPC_HEADERS}
        COMMAND protobuf::protoc
        ARGS
        --grpc_out "${API_GENERATED_DIR}"
        --cpp_out "${API_GENERATED_DIR}"
        -I"${PROTO_INCLUDE_DIR}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
        ${PROTO_FILES}
        DEPENDS ${PROTO_FILES})

add_custom_target(generate
        DEPENDS
        ${PROTO_GENERATED_SOURCES}
        ${PROTO_GENERATED_HEADERS}
        ${PROTO_GENERATED_GRPC_SOURCES}
        ${PROTO_GENERATED_GRPC_HEADERS}
        )

#SQLite3
add_library(database SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/SQLite3/sqlite3.c)

target_include_directories(database
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SQLite3)

target_link_libraries(database
        PUBLIC Threads::Threads
        dl)

# Blockchain
add_library(blockchain INTERFACE)

target_include_directories(blockchain
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(blockchain
        INTERFACE picosha)

# Service Mainer
add_executable(service_mainer
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/DBConnection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/Mainer.cpp
        ${PROTO_GENERATED_SOURCES}
        ${PROTO_GENERATED_GRPC_SOURCES})

target_include_directories(service_mainer PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${API_GENERATED_DIR})

target_link_libraries(service_mainer
        PUBLIC gRPC::grpc++ gRPC::grpc++_reflection
        protobuf::libprotobuf
        database
        blockchain
        PRIVATE picosha)

# Test
add_executable(tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
        ${PROTO_GENERATED_SOURCES})

target_link_libraries(tests
        PRIVATE GTest::gtest_main
        protobuf::libprotobuf)

target_include_directories(tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/headers
        ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()
add_test(NAME unit_tests COMMAND tests)


if(BUILD_COVERAGE)
    function(ENABLE_PROJECT_COVERAGE PROJECT)
        target_compile_options(${PROJECT} PRIVATE --coverage)
        target_link_libraries(${PROJECT} PRIVATE --coverage)
        add_coverage(${PROJECT})
    endfunction()

    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(codecov)
    enable_project_coverage(service_mainer)
    if ("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
        enable_project_coverage(tests)
    endif()
    list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
    coverage_evaluate()
endif()
