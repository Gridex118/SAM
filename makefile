test.out: src/vm/vm.c tests/logic.c
	gcc -Wall -O1 src/vm/vm.c tests/logic.c -o test.out