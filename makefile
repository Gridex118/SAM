GCC_ARGS = -Wall -O0 -g
GPP_ARGS = -Wall

VM_SRC = src/vm/vm.c src/vm/main_util.c src/vm/main.c
SASM_SRC = src/sasm/lexer.cpp src/sasm/main.cpp

all: vm.out sasm.out

vm.out: $(VM_SRC)
	gcc $(GCC_ARGS) $(VM_SRC) -o vm.out

sasm.out: $(SASM_SRC)
	g++ $(GPP_ARGS) $(SASM_SRC) -o sasm.out