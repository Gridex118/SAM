arith_test.out: src/vm/vm.c tests/arith.c
	gcc -Wall src/vm/vm.c tests/arith.c -o arith_test.out