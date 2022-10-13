#include "parser.hpp"
#include "../vm/vm.h"

using namespace parse;

std::unordered_map<std::string, OPCODE> OPCODE_MAP = {
    {"push", OPCODE::PUSH}, {"pop", OPCODE::POP}, {"dup", OPCODE::DUP},
    {"arith", OPCODE::ARITH}, {"bshift", OPCODE::BSHIFT}, {"logic", OPCODE::LOGIC},
    {"compare", OPCODE::COMPARE}, {"loadr", OPCODE::LOADR}, {"loadm", OPCODE::LOADM},
    {"storer", OPCODE::STORER}, {"storem", OPCODE::STOREM}, {"jmp", OPCODE::JMP},
    {"io", OPCODE::IO}, {"funct", OPCODE::FUNCT}, {"halt", OPCODE::HALT}
};

bool is_opcode(const std::string &candidate) {
    return (OPCODE_MAP.find(candidate) != OPCODE_MAP.end());
}

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

int match_parameter(const std::string &candidate){
    if (candidate == "high") return BYTE_ORDERS::HIGH;
    else if (candidate == "low") return BYTE_ORDERS::LOW;
    else if (candidate == "add") return ARITHMETIC_OPERATIONS::ADD;
    else if (candidate == "sub") return ARITHMETIC_OPERATIONS::SUB;
    else if (candidate == "mult") return ARITHMETIC_OPERATIONS::MULT;
    else if (candidate == "div") return ARITHMETIC_OPERATIONS::DIV;
    else if (candidate == "incr") return ARITHMETIC_OPERATIONS::INCR;
    else if (candidate == "and") return LOGICAL_OPERATIONS::AND;
    else if (candidate == "or") return LOGICAL_OPERATIONS::OR;
    else if (candidate == "not") return LOGICAL_OPERATIONS::NOT;
    else if (candidate == "left") return BSHIFT_PARA::LEFT;
    else if (candidate == "right") return BSHIFT_PARA::RIGHT;
    else if (candidate == "eql") return COMPARISON_PARA::EQL;
    else if (candidate == "neql") return COMPARISON_PARA::NEQL;
    else if (candidate == "ls") return COMPARISON_PARA::LS;
    else if (candidate == "gr") return COMPARISON_PARA::GR;
    else if (candidate == "input") return IO_PARA::INPUT;
    else if (candidate == "print") return IO_PARA::PRINT;
    else if (candidate == "print_eseq") return IO_PARA::PRINT_ESEQ;
    else if (candidate == "integer") return IO_TYPES::INTEGER;
    else if (candidate == "string") return IO_TYPES::STRING;
    else if (candidate == "char") return IO_TYPES::CHAR;
    else if (candidate == "newline") return ESCAPE_SEQUENCES::NEWLINE;
    else if (candidate == "return_carriage") return ESCAPE_SEQUENCES::RETURN_CARRIAGE;
    else if (candidate == "uncond") return JUMP_OPTIONS::UNCOND;
    else if (candidate == "cond") return JUMP_OPTIONS::COND;
    else if (candidate == "call") return FUNCT_OPTIONS::CALL;
    else if (candidate == "return") return FUNCT_OPTIONS::RETURN;
    else if (candidate == "Ra") return REGISTERS::Ra;
    else if (candidate == "Rb") return REGISTERS::Rb;
    else if (candidate == "Rc") return REGISTERS::Rc;
    else if (candidate == "Rsp") return REGISTERS::Rsp;
    else if (candidate == "Rip") return REGISTERS::Rip;
    else if (candidate == "Rcom") return REGISTERS::Rcom;
    else if (candidate == "Rhlt") return REGISTERS::Rhlt;
    else if (candidate == "Rerr") return REGISTERS::Rerr;
    else if (candidate == "Rcbindx") return REGISTERS::Rcbindx;
    else if (candidate == "Rvbindx") return REGISTERS::Rvbindx;
    else if (candidate == "var") return MEMORY_SECTIONS::VAR;
    else if (candidate == "code") return MEMORY_SECTIONS::CODE;
    else return -1;
}

inline int match_directive(const std::string &candidate) {
    if (candidate == "SECTION") return DIRECTIVE::SECTION;
    else if (candidate == "LABEL") return DIRECTIVE::LABEL;
    else if (candidate == "INCLUDE") return DIRECTIVE::INCLUDE;
    else return -1;
}
