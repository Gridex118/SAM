#include "./vm.h"
#include <stdio.h>
#include <stdlib.h>

// It has been assumed that the source file contains only hexadecimal numbers

char HEX_DIGITS[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8',
    '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

#define HEX_DIGIT_COUNT 16

typedef struct{
    char char_read;
    unsigned short read_digit, store_inst, store_data;
    uint16_t instr;
    uint8_t instr_digit_index;
} InstrCandidate;

InstrCandidate* init_instr_candidate(){
    InstrCandidate* incand = (InstrCandidate*) malloc(sizeof(InstrCandidate));
    incand->read_digit = FALSE;
    incand->store_inst = FALSE;
    incand->store_data = FALSE;
    incand->instr = 0;
    incand->instr_digit_index = 0;
    return incand;
}

static inline uint8_t hex_to_dec(const char hex_digit){
    for (int i = 0; i < HEX_DIGIT_COUNT; ++i){
        if (hex_digit == HEX_DIGITS[i]){
            return i;
        }
    }
    return -1; // If program reaches this line, we conclude that no character was matched
}

int parse_instr(InstrCandidate* incand){
    uint8_t decimal = hex_to_dec(incand->char_read);
    if (decimal == -1){
        return -1;
    }
    uint8_t shift_count = (4 * (3 - incand->instr_digit_index));
    /* Every digit, since it was converted from a hex digit, costs
        4 bits; we use bit shifting to fix the place values */
    incand->instr += (decimal << shift_count);
    return 0;
}

void toggle_storage_type(InstrCandidate* incand){
    switch(incand->instr){
        case MEM_DUMP_START:
            incand->store_data = TRUE;
            break;
        case MEM_DUMP_END:
            incand->store_data = FALSE;
            break;
        default:
            incand->store_inst = TRUE;
            break;
    }
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
    InstrCandidate* incand = init_instr_candidate();
    while ((incand->char_read = fgetc(source_file)) != EOF){
        if ((incand->char_read == ' ') | (incand->char_read == '\n')){
            // Ignore whitespace
        } else if (incand->char_read == 'x'){
            // Start reading (0x hexadecimal representation)
            incand->read_digit = TRUE;
        } else {
            if (incand->read_digit == TRUE){
                if (parse_instr(incand) == -1){
                    return -1;
                };
                if (incand->instr_digit_index == 3){
                    toggle_storage_type(incand);
                    if (incand->store_inst == TRUE){
                        instructions[instr_set_index++] = incand->instr;
                        // Reset variables
                        incand = init_instr_candidate();
                    } else if (incand->store_data == TRUE){
                        // Store everything between start and end into memory
                    }
                } else {
                    ++(incand->instr_digit_index);
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
