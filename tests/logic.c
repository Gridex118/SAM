// Testing Logical Instructions

#include "../src/vm/vm.h"
#include <string.h>

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 0b100),
    INSTRUCTION(PUSH, 0b110),
    INSTRUCTION(LOGIC, AND),
    INSTRUCTION(BSHIFT, MEGRE_PARA(LEFT, 2)),
    INSTRUCTION(IO, MEGRE_PARA(PRINT, 0)),
    INSTRUCTION(IO, MEGRE_PARA(PRINT_ESEQ, NEWLINE))
};

int main(){
    memcpy(&code_store, &instructions, sizeof(instructions));
    run_machine();
}