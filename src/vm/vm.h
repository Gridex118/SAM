#ifdef __cplusplus
extern "C"{
#endif

#ifndef VM_H
#define VM_H

#include <stdint.h>

#define MEM_CELL_COUNT 0x0200    // 512 memory cells, 2 bytes each - 1KiB
#define MAX_STACK_LENGTH 0x000F

#define len(arr, base_size) (sizeof(arr) / base_size)

enum BOOL{ FALSE, TRUE };

enum OPCODE{
    PUSH, POP, DUP, ARITH,
    LOGIC, BSHIFT, COMPARE,
    LOADM, LOADR, STOREM,
    STORER, JMP, IO,
    FUNCT, HALT
};

#define MAX_PARAMETER_SIZE 12

#define CODE_SECTION_START 0xFFFE
#define MEM_SECTION_START 0xFFFF

enum BYTE_ORDERS{ LOW, HIGH };

enum ARITHMETIC_OPERATIONS{
    ADD, SUB, MULT, DIV, INCR
};

enum LOGICAL_OPERATIONS{ AND, OR, NOT };

enum BSHIFT_PARA{ LEFT, RIGHT };

enum COMPARISON_PARA{ EQL, NEQL, LS, GR };

enum IO_PARA{
    INPUT, PRINT, PRINT_ESEQ
};

enum IO_TYPES{ INTEGER, STRING, CHAR };

enum ESCAPE_SEQUENCES{
    NEWLINE, RETURN_CARRIAGE
};

enum JUMP_OPTIONS{ UNCOND, COND };

enum FUNCT_OPTIONS{ CALL, RETURN };

enum REGISTERS{
    Ra, Rb, Rc,
    Rsp, Rip, Rcom, 
    Rbindx, Rhlt, Rerr,
    R_COUNT,
    Rcbindx, Rvbindx
};

enum ERROR_CODES{
    ILLEGAL_PARAMETER = 1, REGISTER_ACCESS_DENIED, STACK_UNDERFLOW,
    STACK_OVERFLOW, ZERO_DIV_ERROR
};

enum MEMORY_SECTIONS{ VAR, CODE };

extern uint16_t data_store[MEM_CELL_COUNT/2];
extern uint16_t code_store[MEM_CELL_COUNT/2];

void run_machine();

#endif

#ifdef __cplusplus
}
#endif
