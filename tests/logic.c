// Testing Logical Instructions

#include "../src/vm/vm.h"

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 0b100),
    INSTRUCTION(PUSH, 0b110),
    INSTRUCTION(LOGIC, AND),
    INSTRUCTION(BSHIFT, MEGRE_PARA(LEFT, 2)),
    INSTRUCTION(IO, MEGRE_PARA(PRINT, INTEGER)),
    INSTRUCTION(IO, MEGRE_PARA(PRINT_ESEQ, NEWLINE))
};

int main(){
    copy_instructions_to_memory(instructions, sizeof(instructions));
    run_machine();
}