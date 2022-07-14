vm.out: src/vm/vm.c src/vm/main.c
	gcc -Wall -O1 src/vm/vm.c src/vm/main.c -o test.out