cmake_minimum_required(VERSION 3.10)
project(SAM LANGUAGES C CXX)
add_executable(sam.o src/vm/vm.c src/vm/main_util.c src/vm/main.c)
add_executable(sasm.o src/sasm/tokenizer.cpp src/sasm/parser.cpp src/sasm/assembler.cpp src/sasm/main.cpp)
