TEST_FILE = misc

test.out: src/vm/vm.c tests/$(TEST_FILE).c
	gcc -Wall -O1 src/vm/vm.c tests/$(TEST_FILE).c -o test.out