#include "../src/vm/vm.h"

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 122),
    INSTRUCTION(STOREM, 0),
    INSTRUCTION(PUSH, 22),
    INSTRUCTION(DUP, 0),
    INSTRUCTION(LOADM, 0),
    INSTRUCTION(ARITH, MULT),
    INSTRUCTION(IO, MEGRE_PARA(PRINT, INTEGER)),
    INSTRUCTION(IO, MEGRE_PARA(PRINT_ESEQ, NEWLINE))
};

int main(){
    copy_instructions_to_memory(instructions, sizeof(instructions));
    run_machine();
}