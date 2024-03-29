cmake_minimum_required(VERSION 3.10)
project(SAM C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SAM_SOURCE
    src/vm/vm.c
    src/vm/main_util.c
    src/vm/main.c
    )

set(SASM_SOURCE
    src/sasm/tokenizer.cpp
    src/sasm/parser.cpp
    src/sasm/assembler.cpp
    src/sasm/main.cpp
    )

add_executable(sam.out ${SAM_SOURCE})
add_executable(sasm.out ${SASM_SOURCE})

target_compile_options(sam.out PRIVATE -Wall -O0 -g)
target_compile_options(sasm.out PRIVATE -Wall -O0 -g)
