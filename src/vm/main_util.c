#include "main_util.h"
#include "vm.h"
#include <stdio.h>

// It has been assumed that the source file contains only hexadecimal numbers

void init_instr_candidate(InstrCandidate *incand){
    incand->read_digit = FALSE;
    incand->skip_instr = FALSE;
    incand->instr = 0;
    incand->instr_digit_index = 0;
}

static inline uint8_t hex_to_dec(const char hex_digit){
    if ((hex_digit >= '0') && (hex_digit <= '9')) {
        return (hex_digit - '0');
    } else if ((hex_digit >= 'A') && (hex_digit <= 'F')) {
        return (10 + (hex_digit - 'A'));
    } else if ((hex_digit >= 'a') && (hex_digit <= 'f')) {
        return (10 + (hex_digit - 'a'));
    } else return -1;
}

void toggle_storage_type(InstrCandidate *incand){
    incand->skip_instr = TRUE;
    switch(incand->instr){
        case MEM_SECTION_START:
            incand->type = DATA;
            break;
        case CODE_SECTION_START:
            incand->type = INSTR;
            break;
        default:
            /* All other cases correspond to either data or actual
               instructions, and thus must not be skipped */
            incand->skip_instr = FALSE;
            break;
    }
}

void store_instr(InstrCandidate *incand, Indices *indices){
    toggle_storage_type(incand);
    if ((incand->type == INSTR) && !(incand->skip_instr)){
        code_store[(indices->code_indx)++] = incand->instr;
    } else if ((incand->type == DATA) && !(incand->skip_instr)){
        data_store[(indices->data_indx)++] = incand->instr;
    }
}

int parse_instr(InstrCandidate *incand, Indices *indices){
    uint8_t decimal = hex_to_dec(incand->char_read);
    if (decimal == -1){ return -1; }
    uint8_t shift_count = (4 * (3 - incand->instr_digit_index));
    /* Every digit, since it was converted from a hex digit, costs
        4 bits; we use bit shifting to fix the place values */
    incand->instr += (decimal << shift_count);
    if (incand->instr_digit_index == 3){
        store_instr(incand, indices);
        init_instr_candidate(incand);
    } else {
        ++(incand->instr_digit_index);
    }
    if (incand->skip_instr){
        incand->skip_instr = FALSE;
    }
    return 0;
}

int read_source(const char *file_name){
    Indices indices = {0, 0};
    FILE* source_file;
    if ((source_file = fopen(file_name, "r")) == NULL){ return -1; }
    InstrCandidate incand;
    init_instr_candidate(&incand);
    while ((incand.char_read = fgetc(source_file)) != EOF){
        if (is_whitespace(incand.char_read)){
            // Ignore whitespace
        } else if (incand.char_read == 'x'){
            // Start reading (0x hexadecimal representation)
            incand.read_digit = TRUE;
        } else {
            if (incand.read_digit){
                if (parse_instr(&incand, &indices) == -1){ return -1; }
            }
        }
    }
    return 0;
}
