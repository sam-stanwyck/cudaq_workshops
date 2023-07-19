cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(cudaq LANGUAGES CXX C)


find_package(LLVM 16 CONFIG)
message(STATUS " HELLO ${LLVM_BINARY_DIR}")
if(NOT MLIR_DIR)
  set(MLIR_DIR ${LLVM_BINARY_DIR}/lib/cmake/mlir)
endif()

find_package(MLIR REQUIRED CONFIG)

set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/bin)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/lib)
set(MLIR_BINARY_DIR ${CMAKE_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

include(TableGen)
include(AddLLVM)
include(AddMLIR)
include(HandleLLVMOptions)
include_directories(SYSTEM
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
  ${MLIR_INCLUDE_DIRS})
  
add_llvm_pass_plugin(MyCustomPass MyCustomPass.cpp)
target_include_directories(MyCustomPass PRIVATE /workspaces/cuda-quantum/include)

get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)
get_property(conversion_libs GLOBAL PROPERTY MLIR_CONVERSION_LIBS)

target_link_directories(MyCustomPass PRIVATE /workspaces/cuda-quantum/builds/gcc-12-debug/lib)
target_link_libraries(MyCustomPass PRIVATE ${dialect_libs} ${conversion_libs} QuakeDialect)
