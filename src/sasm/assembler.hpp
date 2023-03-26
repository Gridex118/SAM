#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "parser.hpp"
#include "../vm/vm.h"

namespace assemble {
    
    class SamAssembler {
        private:
            std::ofstream sink;
            std::string source_file;
            inline int write(uint16_t instruction);
        public:
            SamAssembler(char* source_file, char* sink_file);
            int assemble();
    };

}

#endif

