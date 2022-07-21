CARGS = -Wall -O0 -g

SRC = src/vm/vm.c src/vm/main_util.c src/vm/main.c

TARGET = vm.out

$(TARGET): $(SRC)
	gcc $(CARGS) $(SRC) -o $(TARGET)