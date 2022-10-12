C_ARGS = -Wall -O0 -g
CPP_ARGS = -Wall -O0 -g

VM_SRC = src/vm/vm.c \
	 src/vm/main_util.c \
	 src/vm/main.c

all: vm.out

vm.out: $(VM_SRC)
	gcc $(C_ARGS) $(VM_SRC) -o vm.out
