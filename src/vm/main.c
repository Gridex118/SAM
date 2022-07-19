#include "./vm.h"
#include "./main_util.h"
#include <stdio.h>
#include <stdlib.h>

// It has been assumed that the source file contains only hexadecimal numbers

char HEX_DIGITS[HEX_DIGIT_COUNT] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8',
    '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

InstrCandidate* init_instr_candidate(){
    InstrCandidate* incand = (InstrCandidate*) malloc(sizeof(InstrCandidate));
    incand->read_digit = FALSE;
    incand->is_instr = FALSE;
    incand->is_data = FALSE;
    incand->skip_instr = FALSE;
    incand->instr = 0;
    incand->instr_digit_index = 0;
    return incand;
}

InstrCandidate* reset_incand(InstrCandidate *old_incand){
    InstrCandidate *incand = init_instr_candidate();
    incand->is_data = old_incand->is_data;
    incand->is_instr = old_incand->is_instr;
    return incand;
}

inline uint8_t hex_to_dec(const char hex_digit){
    for (int i = 0; i < HEX_DIGIT_COUNT; ++i){
        if (hex_digit == HEX_DIGITS[i]){
            return i;
        }
    }
     // If program reaches this point, we conclude that no character was matched
    return -1;
}

int parse_instr(InstrCandidate *incand){
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

void toggle_storage_type(InstrCandidate *incand){
    incand->skip_instr = TRUE;
    switch(incand->instr){
        case MEM_DUMP_START:
            incand->is_data = TRUE;
            break;
        case MEM_DUMP_END:
            incand->is_data = FALSE;
            break;
        case CODE_DUMP_START:
            incand->is_instr = TRUE;
            break;
        case CODE_DUMP_END:
            incand->skip_instr = FALSE;
            break;
        default:
            /* All other cases correspond to either data or actual
               instructions, and thus must not be skipped */
            incand->skip_instr = FALSE;
    }
}

int read_source(const char *file_name){
    uint16_t instructions[MEM_CELL_COUNT/2];
    uint16_t data[MEM_CELL_COUNT/2];
    /* The maximum number of elements in the array must be equal to
       the maximum number of instructions or data that can be stored */
    uint16_t instr_set_index = 0;
    uint16_t data_index = 0;
    FILE* source_file;
    if ((source_file = fopen(file_name, "r")) == NULL){
        return -1;
    }
    InstrCandidate* incand = init_instr_candidate();
    while ((incand->char_read = fgetc(source_file)) != EOF){
        if (is_whitespace(incand->char_read)){
            // Ignore whitespace
        } else if (incand->char_read == 'x'){
            // Start reading (0x hexadecimal representation)
            incand->read_digit = TRUE;
        } else {
            if (incand->read_digit){
                if (parse_instr(incand) == -1){
                    return -1;
                }
                if (incand->instr_digit_index == 3){
                    toggle_storage_type(incand);
                    if ((incand->is_instr) && !(incand->skip_instr)){
                        instructions[instr_set_index++] = incand->instr;
                    } else if ((incand->is_data) && !(incand->skip_instr)){
                        data[data_index++] = incand->instr;
                    }
                    incand = reset_incand(incand);
                } else {
                    ++(incand->instr_digit_index);
                }
                if (incand->skip_instr == TRUE){
                    incand->skip_instr = FALSE;
                }
            }
        }
    }
    size_t instr_set_size = (sizeof(uint16_t) * (++instr_set_index));
    copy_instructions_to_memory(instructions, instr_set_size);
    size_t data_size = (sizeof(uint16_t) * (++data_index));
    copy_data_to_memory(data, data_size);
    return 0;
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Syntax: vm [source_file]\n");
        return -1;
    } else {
        if (read_source(argv[1]) != -1){
            run_machine();
        } else {
            printf("Sorry, an error occured\n");
            return -1;
        }
    }
    return 0;
}
