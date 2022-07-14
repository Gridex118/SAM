// Testing arithmetic instructions

#include "../src/vm/vm.h"

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 12),
    INSTRUCTION(PUSH, 132),
    INSTRUCTION(ARITH, ADD),
    INSTRUCTION(PUSH, 2),
    INSTRUCTION(ARITH, MULT),
    INSTRUCTION(IO, MEGRE_PARA(PRINT, INTEGER)),
    INSTRUCTION(IO, MEGRE_PARA(PRINT_ESEQ, 0)),
    INSTRUCTION(HALT, 0)
};

int main(){
    copy_instructions_to_memory(instructions, sizeof(instructions));
    run_machine();
    return 0;
}
