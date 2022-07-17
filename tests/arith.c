// Testing arithmetic instructions

#include "./tests.h"

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 12),
    INSTRUCTION(PUSH, 132),
    INSTRUCTION(ARITH, ADD),
    INSTRUCTION(PUSH, 2),
    INSTRUCTION(ARITH, MULT),
    INSTRUCTION(IO, MERGE_PARA66(PRINT, INTEGER)),
    INSTRUCTION(IO, MERGE_PARA66(PRINT_ESEQ, 0)),
    INSTRUCTION(HALT, 0)
};

int main(){
    copy_instructions_to_memory(instructions, sizeof(instructions));
    run_machine();
    return 0;
}
