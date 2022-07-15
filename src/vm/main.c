#include "./vm.h"
#include <stdio.h>

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

int read_instructions(char* file_name){
    uint16_t instructions[MEM_CELL_COUNT/2];
    /* The maximum number of elements in the array must be equal to
       the maximum number of instructions that can be stored */
    uint16_t instr_set_index = 0;
    FILE* source_file = fopen(file_name, "r");
    char char_read;
    unsigned short add_digit = FALSE;
    uint16_t instr = 0;
    uint8_t instr_digit_indx = 0;
    while ((char_read = fgetc(source_file)) != EOF){
        if ((char_read == ' ') | (char_read == '\n')){
            // Ignore whitespace
        } else if (char_read == 'x'){
            add_digit = TRUE;
        } else {
            if (add_digit == TRUE){
                uint8_t digit = hex_to_dec(char_read);
                if (digit == -1){
                    return -1;
                }
                switch(instr_digit_indx++){
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
                        // Reset instruction
                        add_digit = FALSE;
                        instr = 0;
                        instr_digit_indx = 0;
                        break;
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
            printf("Sorry, an error Occured\n");
        }
    }
    return 0;
}
