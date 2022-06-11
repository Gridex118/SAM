#include <stdint.h>

#ifndef VM_H
#define VM_H

#define MEM_CELL_COUNT 0x0200    // 512 memory cells, 2 bytes each - 1KiB
#define MAX_STACK_LENGTH 0x0014

#define INSTRUCTION(OPCODE, PARA) ((OPCODE << 12) + PARA)
#define MEGRE_PARA(PARA1, PARA2) ((PARA1 << 6) + PARA2)

enum BOOL{
    FALSE, TRUE
};

enum OPCODE{
    PUSH,    // Push Data into Stack 
    POP,    // Pop Data from Stock into Register 
    DUP,    // Duplicate Stack[SP-1]
    ARITH,    // Handle Arithmetic Operations 
    AND,    // Stack[SP-1] & Stack[SP-2] 
    OR,    // Stack[SP-1] | Stack[SP-2] 
    NOT,    // ~Stack[SP-1] 
    BSHIFT,    // Handle Bit Shift 
    COMP,    // Compare Stack[SP-1] and Stack[SP-2] 
    LOADM,    // Load from Memory to Top of the Stack 
    LOADR,    // Load from Register to Top of Stack 
    STOREM,    // Store into Memory from Top of the Stack 
    STORER,    // Store into Register from Top of the Stack 
    GOTO,    // Goto Rcbindx + Stack[SP-1] - Conditional if Parameter = True 
    IO,    // Handle Input or Output 
    HALT    // Stop Execution 
};

enum ARITHMETIC{
    ADD, SUB, MULT, DIV
};

enum IO_PARA{
    INPUT, OUTPUT, ESEQ
};

enum OUTPUT{
    INTEGER, STRING
};

enum ESCAPE_SEQUENCES{
    NEWLINE = 0, RETURN_CARRIAGE
};

enum REGISTERS{
    Ra,
    Rb,    // a, b, c - General Purpose Registers 
    Rc,
    Rsp,    // Stack Pointer 
    Rip,    // Instruction Pointer 
    Rcom,    // Comparison Data Storage 
    Rcbindx,    // Code Base Index 
    Rhlt,    // Machine Halt Status 
    R_COUNT    // Number of Registers 
};

uint16_t stack[MAX_STACK_LENGTH];
uint16_t var_store[MEM_CELL_COUNT/2];
uint16_t code_store[MEM_CELL_COUNT/2];
uint16_t reg_data[R_COUNT];

#define IP reg_data[Rip]
#define SP reg_data[Rsp]
#define CODE_BASE_INDEX reg_data[Rcbindx]

#define MACHINE_IS_RUNNING (reg_data[Rhlt] == FALSE)

#define OPCODE(instruction) (instruction >> 12)

void execute_instruction(uint16_t instruction);
void run_machine();

#endif
