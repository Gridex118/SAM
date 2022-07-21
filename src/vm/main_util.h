#include <stdint.h>

#define HEX_DIGIT_COUNT 16

extern char HEX_DIGITS[HEX_DIGIT_COUNT];

typedef struct{
    char char_read;
    unsigned short read_digit, is_instr, is_data, skip_instr;
    uint16_t instr;
    uint8_t instr_digit_index;
} InstrCandidate;

int read_source(const char *file_name);

#define is_whitespace(character) ((character == ' ') || (character == '\n'))
