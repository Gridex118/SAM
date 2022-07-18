#include "./vm.h"
#include <stdio.h>

// It has been assumed that the source file contains only hexadecimal numbers

char HEX_DIGITS[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8',
    '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

#define HEX_DIGIT_COUNT 16

static inline uint8_t hex_to_dec(const char hex_digit){
    for (int i = 0; i < HEX_DIGIT_COUNT; ++i){
        if (hex_digit == HEX_DIGITS[i]){
            return i;
        }
    }
    return -1; // If program reaches this line, we conclude that no character was matched
}

int read_instructions(const char* file_name){
    uint16_t instructions[MEM_CELL_COUNT/2];
    /* The maximum number of elements in the array must be equal to
       the maximum number of instructions that can be stored */
    uint16_t instr_set_index = 0;
    FILE* source_file;
    if ((source_file = fopen(file_name, "r")) == NULL){
        return -1;
    }
    char char_read;
    unsigned short read_digit = FALSE;
    unsigned short store_inst = FALSE;
    unsigned short store_data = FALSE;
    uint16_t instr = 0;
    uint8_t instr_digit_indx = 0;
    while ((char_read = fgetc(source_file)) != EOF){
        if ((char_read == ' ') | (char_read == '\n')){
            // Ignore whitespace
        } else if (char_read == 'x'){
            // Start reading (0x hexadecimal representation)
            read_digit = TRUE;
        } else {
            if (read_digit == TRUE){
                uint8_t decimal = hex_to_dec(char_read);
                if (decimal == -1){
                    return -1;
                }
                uint8_t shift_count = (4 * (3 - instr_digit_indx));
                /* Every digit, since it was converted from a hex digit, costs
                   4 bits; we use bit shifting to fix the place values */
                instr += (decimal << shift_count);
                if (instr_digit_indx == 3){
                    if (instr == MEM_DUMP_START){
                        store_data = TRUE;
                    } else if (instr == MEM_DUMP_END){
                        store_data = FALSE;
                    } else{
                        store_inst = TRUE;
                    }
                    if (store_inst == TRUE){
                        instructions[instr_set_index++] = instr;
                        // Reset variables
                        read_digit = FALSE;
                        store_inst = FALSE;
                        instr = 0;
                        instr_digit_indx = 0;
                    } else if (store_data == TRUE){
                        // Store everything between start and end into memory
                    }
                } else {
                    ++instr_digit_indx;
                }
            }
        }
    }
    size_t instr_set_size = (sizeof(uint16_t) * (++instr_set_index));
    copy_instructions_to_memory(instructions, instr_set_size);
    return 0;
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Syntax: vm [source_file]\n");
        return -1;
    } else {
        if (read_instructions(argv[1]) != -1){
            run_machine();
        } else {
            printf("Sorry, an error occured\n");
            return -1;
        }
    }
    return 0;
}
