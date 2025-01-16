# Minimum CMake required
cmake_minimum_required(VERSION 3.16)

# Project
project(math_expr)

# --- ^&^ --- #
set(PROTO_CPP_OUT "./proto")
set(PROTO_GRPC_OUT "./proto")
set(PROTO_PATH "../../shared/config/proto")

# Protobuf
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

# Protobuf-compiler
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(hw_proto "${PROTO_PATH}/mathexpr.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)
get_filename_component(PROTO_PATH "${PROTO_PATH}" ABSOLUTE)

# Generated sources
set(hw_proto_srcs "${CMAKE_SOURCE_DIR}/proto/mathexpr.pb.cc")
set(hw_proto_hdrs "${CMAKE_SOURCE_DIR}/proto/mathexpr.pb.h")
set(hw_grpc_srcs "${CMAKE_SOURCE_DIR}/proto/mathexpr.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_SOURCE_DIR}/proto/mathexpr.grpc.pb.h")

add_custom_command(
    OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --proto_path="${PROTO_PATH}"
    --grpc_out "${CMAKE_SOURCE_DIR}/${PROTO_GRPC_OUT}"
    --cpp_out "${CMAKE_SOURCE_DIR}/${PROTO_CPP_OUT}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    "${hw_proto}"
    DEPENDS "${hw_proto}"
)

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}/proto")

# Targets (client|server)
foreach(
    _target
    client server
)    
    add_executable(
        ${_target} "${_target}.cc"
        ${hw_proto_srcs}
        ${hw_grpc_srcs}
    )
    
    target_link_libraries(
        ${_target}
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
    )
endforeach()
