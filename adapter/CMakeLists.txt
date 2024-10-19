cmake_minimum_required(VERSION 3.10.2)
project(adapter)

set(CMAKE_CXX_STANDARD 17)

#proto
set(PROTOS
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/status.proto)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-src)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindGRPC.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindProtobuf.cmake)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})

# <------------ add hiredis dependency --------------->
find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)

# <------------ add redis-plus-plus dependency -------------->
# NOTE: this should be *sw* NOT *redis++*
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

#src
file(GLOB_RECURSE sources "src/[a-zA-Z]*.cpp")
file(GLOB_RECURSE public_headers "include/${PROJECT_NAME}/[a-zA-Z]*.h")
file(GLOB_RECURSE private_headers "src/[a-zA-Z]*.h")

set(library_sources
        ${sources}
        ${public_headers}
        ${private_headers}
)

add_library(${PROJECT_NAME} STATIC
        ${PROTO_SRCS}
        ${GRPC_SRCS}
        ${library_sources})

target_include_directories(${PROJECT_NAME}
        PUBLIC include
        PRIVATE ${PROTO_SRC_DIR} include/${PROJECT_NAME} ${HIREDIS_HEADER} ${REDIS_PLUS_PLUS_HEADER})

find_package(OpenSSL REQUIRED)

target_link_libraries(${PROJECT_NAME}
        application
        fastlog
        etcdregistry
        gRPC::grpc++_reflection
        protobuf::libprotobuf
        ${HIREDIS_LIB}
        ${REDIS_PLUS_PLUS_LIB})