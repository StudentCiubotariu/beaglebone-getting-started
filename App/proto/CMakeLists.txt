cmake_minimum_required(VERSION 3.16)

add_library(proto STATIC)

find_package(Protobuf REQUIRED)

# Input .proto and output directory (source tree)
set(PROTO_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/message.proto")
set(PROTO_GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")

file(MAKE_DIRECTORY "${PROTO_GEN_DIR}")

set(GEN_CC "${PROTO_GEN_DIR}/message.pb.cc")
set(GEN_H  "${PROTO_GEN_DIR}/message.pb.h")

add_custom_command(
  OUTPUT "${GEN_CC}" "${GEN_H}"
  COMMAND protoc
          --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
          --cpp_out=${PROTO_GEN_DIR}
          ${PROTO_FILE}
  DEPENDS "${PROTO_FILE}"
  COMMENT "Generating protobuf C++: ${PROTO_FILE}"
  VERBATIM
)

# Ensure generation runs when building proto (more explicit/robust)
add_custom_target(proto_gen DEPENDS "${GEN_CC}" "${GEN_H}")
add_dependencies(proto proto_gen)

target_sources(proto PRIVATE "${GEN_CC}")
target_include_directories(proto PUBLIC "${PROTO_GEN_DIR}")
target_link_libraries(proto PUBLIC protobuf::libprotobuf)
