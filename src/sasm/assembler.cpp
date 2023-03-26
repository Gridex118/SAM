#include "assembler.hpp"
#include <iostream>
#include <iomanip>

using namespace assemble;
using namespace parse;

inline int SamAssembler::write(uint16_t instruction) {
    sink << "0x" << std::setfill('0') << std::setw(4)
         << std::hex << instruction << '\n';
}

SamAssembler::SamAssembler(char* source_file, char* sink_file) : source_file(source_file) {
    sink.open(sink_file);
}

int SamAssembler::assemble {
    Parser parser(source_file);
    ParserOutputContainer* parser_output = parser.parse();
}
