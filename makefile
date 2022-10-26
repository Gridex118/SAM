C_ARGS = -Wall -O0 -g
CPP_ARGS = -Wall -O0 -g

VM_SRC = src/vm/vm.c \
	 src/vm/main_util.c \
	 src/vm/main.c

TOKENIZER_TEST_SRC = src/sasm/parser_utils.cpp \
		     src/sasm/tokenizer.cpp \
		     tests/tokenizer_test.cpp

all: vm.out

vm.out: $(VM_SRC)
	gcc $(C_ARGS) $(VM_SRC) -o vm.out

tests: tokenizer_test.out

tokenizer_test.out: $(TOKENIZER_TEST_SRC)
	g++ $(CPP_ARGS) $(TOKENIZER_TEST_SRC) -o tokenizer_test.out
