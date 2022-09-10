C_ARGS = -Wall -O0 -g
CPP_ARGS = -Wall -O0 -g

VM_SRC = src/vm/vm.c \
	 src/vm/main_util.c \
	 src/vm/main.c

SASM_SRC = src/sasm/token_container.cpp \
	   src/sasm/lexer.cpp \
	   src/sasm/pars_err_rep.cpp\
	   src/sasm/parser.cpp \
	   src/sasm/main.cpp

all: vm.out sasm.out

vm.out: $(VM_SRC)
	gcc $(C_ARGS) $(VM_SRC) -o vm.out

sasm.out: $(SASM_SRC)
	g++ $(CPP_ARGS) $(SASM_SRC) -o sasm.out
