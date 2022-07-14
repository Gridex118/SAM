#include "../src/vm/vm.h"

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 1),
    INSTRUCTION(DUP, 0),
    INSTRUCTION(IO, MEGRE_PARA(PRINT, INTEGER)),
    INSTRUCTION(IO, MEGRE_PARA(PRINT_ESEQ, NEWLINE)),
    INSTRUCTION(PUSH, 1),
    INSTRUCTION(ARITH, ADD),
    INSTRUCTION(DUP, 0),
    INSTRUCTION(PUSH, 0x0FFF),
    INSTRUCTION(BSHIFT, MEGRE_PARA(LEFT, 4)),
    INSTRUCTION(PUSH, 0x000F),
    INSTRUCTION(ARITH, ADD),
    INSTRUCTION(COMPARE, LS),
    INSTRUCTION(PUSH, 0),
    INSTRUCTION(JMP, COND),
    INSTRUCTION(HALT, 0)
};

int main(){
    copy_instructions_to_memory(instructions, sizeof(instructions));
    run_machine();
    return 0;
}