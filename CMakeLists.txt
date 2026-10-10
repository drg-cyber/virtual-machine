cmake_minimum_required(VERSION 3.16)

project(VirtualMachine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(vm
    src/Main.cpp
    src/CPU.cpp
    src/Flags.cpp
    src/Instruction.cpp
    src/Memory.cpp
    src/MyException.cpp
    src/Registers.cpp
    src/Runner.cpp
)

target_include_directories(vm PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/program.asm"
    "${CMAKE_CURRENT_BINARY_DIR}/program.asm"
    COPYONLY
)