#include "./vm.h"
#include <stdio.h>
#include <stdlib.h>

uint16_t* read_instructions(char** file_name){
    uint16_t instructions[MEM_CELL_COUNT/2];
    /* The maximum number of elements in the array must be equal to
       the maximum number of instructions that can be stored */
    uint16_t instr_count = 0;
    uint16_t start_index = 1;
    /* The first element in the array shall contain
       the number of instructions read  */
    FILE* source_file = fopen(file_name, 'r');
    char char_read;
    uint16_t instr;
    uint8_t instr_digit_indx = 0;
    while ((char_read = fgetc(source_file)) != EOF){
        if ((char_read == ' ') | (char_read == '\n')){
            // Ignore whitespace
        }
    }
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Syntax: vm [source_file]");
        return -1;
    } else {
        uint16_t* instructions = read_instructions(argv[1]);
        uint16_t instruction_size = (sizeof(uint16_t) * instructions[0]);
        copy_instructions_to_memory(instructions, 10);
        run_machine();
    }
}
