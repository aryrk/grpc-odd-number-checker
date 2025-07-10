cmake_minimum_required(VERSION 3.15)
project(OddNumberChecker)

# list(APPEND CMAKE_PREFIX_PATH "/opt/grpc" "/opt/protobuf")
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(PREPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

find_package(uwebsockets REQUIRED)
find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

set(PROTO_SRC
${CMAKE_CURRENT_SOURCE_DIR}/proto/number_checker.proto
)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.cc src/*.h src/*.hpp)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTO_SRC})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTO_SRC})

add_executable(client src/client.cpp ${SOURCES} ${PROTO_SRCS} ${GRPC_SRCS})
target_include_directories(client PRIVATE ${PROTO_SRC_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(client gRPC::grpc++ protobuf::libprotobuf uwebsockets::uwebsockets)

add_executable(server src/server.cpp ${SOURCES} ${PROTO_SRCS} ${GRPC_SRCS})
target_include_directories(server PRIVATE ${PROTO_SRC_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(server gRPC::grpc++ protobuf::libprotobuf uwebsockets::uwebsockets)
