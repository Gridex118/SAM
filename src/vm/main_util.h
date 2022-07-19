#include <stdint.h>

#define HEX_DIGIT_COUNT 16

extern char HEX_DIGITS[HEX_DIGIT_COUNT];

typedef struct{
    char char_read;
    unsigned short read_digit, is_instr, is_data, skip_instr;
    uint16_t instr;
    uint8_t instr_digit_index;
} InstrCandidate;

inline uint8_t hex_to_dec(const char hex_digit);

#define is_whitespace(character) ((character == ' ') || (character == '\n'))
