/* Testing the instruction processing */

#include "../src/vm/vm.h"
#include <stdlib.h>
#include <string.h>

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 12),
    INSTRUCTION(PUSH, 132),
    INSTRUCTION(ARITH, ADD),
    INSTRUCTION(PUSH, 2),
    INSTRUCTION(ARITH, MULT),
    INSTRUCTION(STORER, Rhlt),
    INSTRUCTION(LOADR, Rerr),
    INSTRUCTION(IO, MEGRE_PARA(OUTPUT, INTEGER)),
    INSTRUCTION(IO, MEGRE_PARA(ESEQ, 0)),
    INSTRUCTION(HALT, 0)
};

int main(){

    memcpy(&code_store, &instructions, sizeof(instructions));
    run_machine();

}
