// Testing Logical Instructions

#include "./tests.h"

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 0b100),
    INSTRUCTION(PUSH, 0b110),
    INSTRUCTION(LOGIC, AND),
    INSTRUCTION(BSHIFT, MERGE_PARA48(LEFT, 2)),
    INSTRUCTION(IO, MERGE_PARA66(PRINT, INTEGER)),
    INSTRUCTION(IO, MERGE_PARA66(PRINT_ESEQ, NEWLINE))
};

int main(){
    copy_instructions_to_memory(instructions, sizeof(instructions));
    run_machine();
    return 0;
}