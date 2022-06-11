/* Testing the instruction processing */

#include "../src/vm/vm.h"
#include <stdlib.h>
#include <string.h>

#define len(arr, elem_size) (sizeof(arr) / elem_size)
#define len_uint16(arr) len(arr, sizeof(uint16_t))

uint16_t instructions[] = {
    INSTRUCTION(PUSH, 12),
    INSTRUCTION(PUSH, 132),
    INSTRUCTION(ARITH, ADD),
    INSTRUCTION(POP, 0),
    INSTRUCTION(IO, MEGRE_PARA(OUTPUT, INTEGER)),
    INSTRUCTION(IO, MEGRE_PARA(ESEQ, 0)),
    INSTRUCTION(HALT, 0)
};

extern uint16_t stack[];
extern uint16_t var_store[];
extern uint16_t code_store[];
extern uint16_t reg_data[];

int main(){

    memcpy(&code_store, &instructions, sizeof(instructions));
    run_machine();
    
}
