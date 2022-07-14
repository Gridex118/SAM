#include "./vm.h"
#include <stdio.h>
#include <stdlib.h>

char HEX_DIGITS[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8',
    '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

#define HEX_DIGIT_COUNT 16

static inline uint8_t hex_to_dec(char hex_digit_char){
    for (int i = 0; i < HEX_DIGIT_COUNT; ++i){
        if (hex_digit_char == HEX_DIGITS[i]){
            return i;
        }
    }
    return -1; // If program reaches this line, we conclude that no character was matched
}

uint16_t* read_instructions(char* file_name){
    uint16_t* instructions = (uint16_t*) malloc(
        sizeof(uint16_t) * (MEM_CELL_COUNT/2)
    );
    /* The maximum number of elements in the array must be equal to
       the maximum number of instructions that can be stored */
    uint16_t instr_count = 0;
    uint16_t instr_set_index = 1;
    /* The first element in the array shall contain
       the number of instructions read  */
    FILE* source_file = fopen(file_name, "r");
    char char_read;
    uint16_t instr = 0;
    uint8_t instr_digit_indx = 0;
    while ((char_read = fgetc(source_file)) != EOF){
        printf("%c\n", char_read);
        if ((char_read == ' ') | (char_read == '\n')){
            // Ignore whitespace
        } else {
            uint8_t digit = hex_to_dec(char_read);
            switch(instr_digit_indx){
                case 0:
                    instr += (digit << 12);
                    break;
                case 1:
                    instr += (digit << 8);
                    break;
                case 2:
                    instr += (digit << 4);
                    break;
                case 3:
                    instr += digit;
                    instructions[instr_set_index++] = instr;
                    ++instr_count;
                    printf("%x\n", instr);
                    // Reset instruction
                    instr = 0;
                    instr_digit_indx = 0;
                    break;
            }
        }
    }
    instructions[0] = instr_count;
    return instructions;
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Syntax: vm [source_file]");
        return -1;
    } else {
        printf("Reading from %s\n\n", argv[1]);
        uint16_t* instructions = read_instructions(argv[1]);
        uint16_t instruction_set_size = (sizeof(uint16_t) * instructions[0]);
        copy_instructions_to_memory(instructions, instruction_set_size);
        run_machine();
    }
}
