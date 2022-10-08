#include "parser.hpp"
#include "../vm/vm.h"
#include <iostream>
#include <iomanip>
#include <assert.h>

using namespace parse;

std::unordered_map<std::string, OPCODE> OPCODE_MAP = {
    {"push", OPCODE::PUSH}, {"pop", OPCODE::POP}, {"dup", OPCODE::DUP},
    {"arith", OPCODE::ARITH}, {"bshift", OPCODE::BSHIFT}, {"logic", OPCODE::LOGIC},
    {"compare", OPCODE::COMPARE}, {"loadr", OPCODE::LOADR}, {"loadm", OPCODE::LOADM},
    {"storer", OPCODE::STORER}, {"storem", OPCODE::STOREM}, {"jmp", OPCODE::JMP},
    {"io", OPCODE::IO}, {"funct", OPCODE::FUNCT}, {"halt", OPCODE::HALT}
};

std::vector<uint16_t>* string_to_words(std::string str){
    std::vector<uint16_t> *words = new std::vector<uint16_t>;
    for (unsigned int i = 0; i < str.length(); i += 2) {
        words->push_back(
            (static_cast<uint8_t>(str[i]) << 8)
            + static_cast<uint8_t>(str[i+1])
        );    // Consume two charcters at a time
    }
    return words;
}

unsigned short parameters_due_for_opcode(int &opcode){
    switch (opcode) {
        // Single parameter instructions
        case OPCODE::PUSH:
        case OPCODE::ARITH:
        case OPCODE::LOGIC:
        case OPCODE::COMPARE:
        case OPCODE::LOADR:
        case OPCODE::STORER:
            return 1;
            break;
        // Double parameter instructions
        case OPCODE::BSHIFT:
        case OPCODE::LOADM:
        case OPCODE::STOREM:
        case OPCODE::JMP:
        case OPCODE::IO:
        case OPCODE::FUNCT:
            return 2;
            break;
        default:
            return 0;
    }
}

inline unsigned short get_second_parameter_size(int opcode){
    switch (opcode) {
        case OPCODE::BSHIFT:
            return 8;
            break;
        case OPCODE::FUNCT:
        case OPCODE::LOADM:   
        case OPCODE::STOREM:
        case OPCODE::JMP:
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

inline int match_directive(const std::string &candidate){
    if (candidate == "SECTION") return DIRECTIVE::SECTION;
    else if (candidate == "LABEL") return DIRECTIVE::LABEL;
    else if (candidate == "INCLUDE") return DIRECTIVE::INCLUDE;
    else return -1;
}

inline void Parser::write(){
    sink << "0x" << std::setfill('0') << std::setw(4)
         << std::hex << instruction << '\n';
    instruction = 0;
}

inline void Parser::write_str_as_bytes(){
    std::vector<uint16_t> *words = string_to_words(current_token->value);
    for (auto i = words->begin(); i < words->end(); ++i){
        instruction = *i;
        write();
    }
    delete words;
}

int Parser::deal_with_directives(){
    int directive_type = match_directive(current_token->value);
    current_token = tokenizer->next_token_to_parse();
    if (directive_type == DIRECTIVE::SECTION) {
        if (current_token->value == "CODE") {
            state.mode = MODE::CODE;
            instruction = CODE_SECTION_START;
            write();
        } else if (current_token->value == "MEM") {
            state.mode = MODE::DATA;
            instruction = MEM_SECTION_START;
            write();
        } else {
            report_section_error(current_token->line);
            return -1;
        }
    } else if (directive_type == DIRECTIVE::LABEL) {
        data[current_token->value] = (state.instruction_count - 1);
        // A 1 must be subtracted since the instruction indexing oughts to start at 0
    } else if (directive_type == DIRECTIVE::INCLUDE) {
        deal_with_imports();
    } else {
        report_directive_error(current_token->line);
        return -1;
    }
    return 0;
}

int Parser::deal_with_opcodes(){
    std::string value = current_token->value;
    if (OPCODE_MAP.find(value) != OPCODE_MAP.end()) {
        int opcode = OPCODE_MAP[value];
        state.instruction_count += 1;
        instruction += (opcode << 12);
        state.parameters_due = parameters_due_for_opcode(opcode);
        state.second_parameter_size = get_second_parameter_size(opcode);
        if (state.parameters_due == 0) write();
        return 0;
    } else return -1;
}

inline int Parser::add_para_to_instr(){
    int base;
    switch (current_token->type) {
        case lex::TOKENS::NUMBER:
            base = stoi(current_token->value);
            break;
        default:
            if ((base = match_parameter(current_token->value)) == -1) {
                if (data.find(current_token->value) == data.end()) {
                    report_parameter_error(current_token->line);
                    return -1;
                } else {
                    base = data[current_token->value];
                }
            }
            break;
    }
    if (state.parameters_due == 2) {
        instruction += (base << state.second_parameter_size);
        state.second_parameter_size = 0;
    } else {
        instruction += base;
    }
    return 0;
}

int Parser::deal_with_num_parameters(){
    assert(
        (state.parameters_due == 2)
        || (
            (state.parameters_due < 2)
            && (state.second_parameter_size == 0)
        )
    );
    if (add_para_to_instr() ==  -1) return -1;
    --state.parameters_due;
    if (state.parameters_due == 0) write();
    return 0;
}

int Parser::deal_with_plain_parameters(){
    assert(
        (state.parameters_due == 2)
        || (
            (state.parameters_due < 2)
            && (state.second_parameter_size == 0)
        )
    );
    if (add_para_to_instr() == -1) return -1;
    --state.parameters_due;
    if (state.parameters_due == 0) write();
    return 0;
}

int Parser::deal_with_imports(){
    lex::Tokenizer *old_tokenizer = tokenizer;
    lex::Tokenizer *new_tokenizer = new lex::Tokenizer(current_token->value);
    tokenizer = new_tokenizer;
    tokenizer->tokenize();
    if (parse() == -1) return -1;
    tokenizer = old_tokenizer;
    delete new_tokenizer;
    return 0;
}

int Parser::parse(){
    while ((current_token = tokenizer->next_token_to_parse()) != NULL){
        switch (current_token->type) {
            case lex::TOKENS::DIRECTIVE:
                if (deal_with_directives() == -1) return -1;
                break;
            case lex::TOKENS::NUMBER:
                if ((state.mode == MODE::CODE) && (state.parameters_due > 0)) {
                    if (deal_with_num_parameters() == -1) return -1;
                    break;
                } else ; // Fall through to case TOKENS::PLAIN, and report error
            case lex::TOKENS::PLAIN:
                if (state.mode != MODE::CODE) {
                    report_instr_outside_code_sec_error(current_token->line);
                    return -1;
                }
                if (deal_with_opcodes() == -1) {
                    if (state.parameters_due == 0){
                        report_syntax_error(current_token->line);
                        return -1;
                    } else {
                        if (deal_with_plain_parameters() == -1) return -1;
                    }
                }
                break;
            case lex::TOKENS::STRING:
                if (state.mode != MODE::DATA) {
                    report_string_outside_data_sec_error(current_token->line);
                    return -1;
                } else {
                    write_str_as_bytes();
                }
        }
    }
    return 0;
}
