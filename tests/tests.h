#include "../src/vm/vm.h"

#define INSTRUCTION(OPCODE, PARA) ((OPCODE << 12) + PARA)
#define MERGE_PARA66(PARA1, PARA2) ((PARA1 << 6) + PARA2)
#define MERGE_PARA48(PARA1, PARA2) ((PARA1 << 8) + PARA2)
