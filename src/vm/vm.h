#ifndef VM_H
#define VM_H

#include <stdint.h>

#define MEM_CELL_COUNT 0x0200    // 512 memory cells, 2 bytes each - 1KiB
#define MAX_STACK_LENGTH 0x0014

#define len(arr, base_size) (sizeof(arr) / base_size)

enum BOOL{
    FALSE, TRUE
};

enum OPCODE{
    PUSH,    // Push Data into Stack 
    POP,    // Pop Data from Stock into Register 
    DUP,    // Duplicate Stack[SP-1]
    ARITH,    // Handle Arithmetic Operations 
    LOGIC,    // Handle Logical Operations
    BSHIFT,    // Handle Bit Shift 
    COMPARE,    // Compare Stack[SP-2] and Stack[SP-1] 
    LOADM,    // Load from Memory to Top of the Stack 
    LOADR,    // Load from Register to Top of Stack 
    STOREM,    // Store into Memory from Top of the Stack
    STORER,    // Store into Register from Top of the Stack 
    JMP,    // Goto Rcbindx + Stack[SP-1] - Conditional if Parameter = COND
    IO,    // Handle Input or Output 
    FUNCT,    // Handle function call and return
    HALT    // Stop Execution 
};

#define CODE_SECTION_START 0xFFFE
#define MEM_SECTION_START 0xFFFF

enum BYTE_ORDERS{ LOW, HIGH };

enum ARITHMETIC_OPERATIONS{
    ADD, SUB, MULT, DIV
};

enum LOGICAL_OPERATIONS{ AND, OR, NOT };

enum BSHIFT_PARA{ LEFT, RIGHT };

enum COMPARISON_PARA{ EQL, LS, GR };

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
    Ra,
    Rb,    // a, b, c - General Purpose Registers 
    Rc,
    Rsp,    // Stack Pointer 
    Rip,    // Instruction Pointer 
    Rcom,    // Comparison Data Storage 
    Rbindx,    // Base Index: VAR CODE
    Rhlt,    // Machine Halt Status
    Rerr,    // Error Code Register
    R_COUNT,    // Number of Registers 
    Rcbindx,    // Code Base Index
    Rvbindx,    // Variable Store Base Index
};

enum ERROR_CODES{
    ILLEGAL_PARAMETER = 1, REGISTER_ACCESS_DENIED, STACK_UNDERFLOW,
    ZERO_DIV_ERROR
};

enum MEMORY_SECTIONS{ VAR, CODE };

extern uint16_t stack[MAX_STACK_LENGTH];
extern uint16_t data_store[MEM_CELL_COUNT/2];
extern uint16_t code_store[MEM_CELL_COUNT/2];
extern uint16_t reg_data[R_COUNT];

#define IP reg_data[Rip]
#define SP reg_data[Rsp]
#define CODE_BASE_INDEX (reg_data[Rbindx] & 0x00FF)

#define MACHINE_IS_RUNNING (reg_data[Rhlt] == FALSE)

#define OPCODE(instruction) (instruction >> 12)

void run_machine();

#endif
