#include <stdint.h>

#define HEX_DIGIT_COUNT 16

extern char HEX_DIGITS[HEX_DIGIT_COUNT];

typedef struct{
    char char_read;
    unsigned short read_digit, type, skip_instr;
    uint16_t instr;
    uint8_t instr_digit_index;
} InstrCandidate;

typedef struct{
    uint16_t data_indx, code_indx;
} Indices;

int read_source(const char *file_name);

enum INSTR_TYPE{
    DATA, INSTR
};

#define is_whitespace(character) ((character == ' ') || (character == '\n'))
