#include "parser.hpp"



unsigned short parameters_due_for_opcode(int &opcode) {
    switch (opcode) {
        // Single parameter instructions
        case OPCODE::PUSH:    case OPCODE::ARITH:
        case OPCODE::LOGIC:   case OPCODE::COMPARE:
        case OPCODE::LOADR:   case OPCODE::STORER:
            return 1;
            break;
        // Double parameter instructions
        case OPCODE::BSHIFT:  case OPCODE::LOADM:
        case OPCODE::STOREM:  case OPCODE::JMP:
        case OPCODE::IO:      case OPCODE::FUNCT:
            return 2;
            break;
        default:
            return 0;
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
    }
}


inline int match_directive(const std::string &candidate) {
    if (candidate == "SECTION") return DIRECTIVE::SECTION;
    else if (candidate == "LABEL") return DIRECTIVE::LABEL;
    else if (candidate == "INCLUDE") return DIRECTIVE::INCLUDE;
    else return -1;
}
