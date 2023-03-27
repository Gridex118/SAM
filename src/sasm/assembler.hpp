#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "parser.hpp"
#include "../vm/vm.h"

namespace assemble {
    
    class SamAssembler {
        private:
            std::ofstream sink;
            char *source_file;
            inline int write();
            inline int write(int instruction);
            inline int write(std::string str);
            struct {
                uint16_t instruction;
                uint8_t parameters_due;
                int opcode;
            } instr_data{ 0, 0, 0 };
        public:
            SamAssembler(char* source_file, char* sink_file);
            int assemble();
    };

}

#endif

