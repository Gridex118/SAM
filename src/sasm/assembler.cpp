#include "assembler.hpp"
#include <iostream>
#include <iomanip>

using namespace assemble;
using namespace parse;

std::vector<uint16_t>* string_to_words(std::string str) {
    std::vector<uint16_t> *words = new std::vector<uint16_t>;
    for (unsigned int i = 0; i < str.length(); i += 2) {
        words->push_back(
            (static_cast<uint8_t>(str[i]) << 8)
            + static_cast<uint8_t>(str[i+1])
        );    // Consume two charcters at a time
    }
    return words;
}

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
