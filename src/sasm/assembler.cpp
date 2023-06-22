#include "assembler.hpp"
#include "assert.h"
#include <iostream>
#include <iomanip>

using namespace assemble;
using namespace parse;

unsigned short parameters_due_for_opcode(int &opcode) {
    switch (opcode) {
        case OPCODE::PUSH:    case OPCODE::ARITH:
        case OPCODE::LOGIC:   case OPCODE::COMPARE:
        case OPCODE::LOADR:   case OPCODE::STORER:
            return 1;
            break;
        case OPCODE::BSHIFT:  case OPCODE::LOADM:
        case OPCODE::STOREM:  case OPCODE::JMP:
        case OPCODE::IO:      case OPCODE::FUNCT:
            return 2;
            break;
        default:
            return 0;
            break;
    }
}

inline unsigned short get_second_parameter_size(int opcode) {
    switch (opcode) {
        case OPCODE::BSHIFT:
            return 8;
            break;
        case OPCODE::FUNCT:    case OPCODE::LOADM:   
        case OPCODE::STOREM:   case OPCODE::JMP:
            return 11;
            break;
        case OPCODE::IO:
            return 6;
            break;
        default:
            return 0;
            break;
    }
}

inline int SamAssembler::write(std::string str) {
    for (unsigned int i = 0; i < str.length(); i += 2) {
        write(
            (static_cast<uint8_t>(str[i]) << 8)
            + static_cast<uint8_t>(str[i+1])
        );    // Consume two charcters at a time
    }
    if (str.length()%2 == 0)
        write (0x0000);
    return 0;
}

inline int SamAssembler::write(int instruction) {
    sink << "0x" << std::setfill('0') << std::setw(4)
         << std::hex << instruction << '\n';
    return 0;
}

inline int SamAssembler::write() {
    write(instr_data.instruction);
    instr_data.instruction = 0;
    return 0;
}
SamAssembler::SamAssembler(char* source_file, char* sink_file) : source_file(source_file) {
    sink.open(sink_file);
}

int SamAssembler::assemble() {
    Parser parser(source_file);
    ParserOutputContainer* parser_output = parser.parse();
    for (auto output_iterator = parser_output->begin(); output_iterator != parser_output->end(); ++output_iterator) {
        ParserOutput* output = *output_iterator;
        bool skip_write = false;
        switch (output->type) {
            case TOKEN::OPCODE_T:
                assert(instr_data.parameters_due == 0);
                instr_data.opcode = output->int_value;
                instr_data.instruction += instr_data.opcode << 12;
                instr_data.parameters_due = parameters_due_for_opcode(instr_data.opcode);
                break;
            case TOKEN::SECTION_DIRECTIVE_T:
                if (output->str_value == "CODE" || output->str_value == "MEM") {
                    instr_data.instruction = output->int_value;
                }
                break;
            case TOKEN::NUMERIC_T:
                if (instr_data.parameters_due == 2) {
                    instr_data.instruction += (output->int_value << get_second_parameter_size(instr_data.opcode));
                    --instr_data.parameters_due;
                } else {
                    instr_data.instruction += output->int_value;
                    --instr_data.parameters_due;
                }
                break;
            case TOKEN::STRING_T:
                skip_write = true;
                write(output->str_value);
                break;
            default:
                return -1;
        }
        if (!skip_write && instr_data.parameters_due == 0) write();
    }
    return 0;
}
