CARGS = -Wall -O1

VM = src/vm/vm.c
MAIN = src/vm/main.c

TARGET = vm.out

$(TARGET): $(VM) $(MAIN)
	gcc $(CARGS) $(VM) $(MAIN) -o $(TARGET)