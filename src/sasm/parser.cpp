#include "parser.hpp"
#include "../vm/vm.h"
#include <iostream>
#include <assert.h>

using namespace std;
using namespace parse;

inline void write(ofstream &sink, uint16_t instruction){
    sink << "0x" << hex << instruction;
}

int match_opcode(const string &candidate){
    if (candidate == "push") return OPCODE::PUSH;
    else if (candidate == "pop") return OPCODE::POP;
    else if (candidate == "dup") return OPCODE::DUP;
    else if (candidate == "arith") return OPCODE::ARITH;
    else if (candidate == "bshift") return OPCODE::BSHIFT;
    else if (candidate == "logic") return OPCODE::LOGIC;
    else if (candidate == "compare") return OPCODE::COMPARE;
    else if (candidate == "loadr") return OPCODE::LOADR;
    else if (candidate == "loadm") return OPCODE::LOADM;
    else if (candidate == "storer") return OPCODE::STORER;
    else if (candidate == "storem") return OPCODE::STOREM;
    else if (candidate == "jmp") return OPCODE::JMP;
    else if (candidate == "io") return OPCODE::IO;
    else if (candidate == "funct") return OPCODE::FUNCT;
    else if (candidate == "halt") return OPCODE::HALT;
    else return -1;
}

inline int match_directive(const string &candidate){
    if (candidate == "SECTION") return DIRECTIVE::SECTION;
    else if (candidate == "LABEL") return DIRECTIVE::LABEL;
    else return -1;
}

inline int match_section(const string &candidate, Parser &parser){
    if (candidate == "CODE") {
        
        return CODE_SECTION_START;
    }
    else if (candidate == "MEM") return MEM_SECTION_START;
    else return -1;
}

int Parser::deal_with_directives(){
    int directive_type = match_directive(current_token->value);
    current_token = tokenizer->next_token_to_parse();
    if (directive_type == DIRECTIVE::SECTION) {
        int section_code = match_section(current_token->value, *this);
        if (section_code != -1) {
            instruction += section_code;
            write(sink, instruction);
        } else {
            cerr << "Illegal section specifier at line ";
            cerr << current_token->line << '\n';
            return -1;
        }
    } else if (directive_type == DIRECTIVE::LABEL) {
        data[current_token->value] = current_token->line;
    } else {
        cerr << "Illegal directive at line ";
        cerr << current_token->line << '\n';
        return -1;
    }
    return 0;
}

int Parser::deal_with_opcodes(){
    int opcode = match_opcode(current_token->value);
    if (opcode != -1) {
        instruction += (opcode << 12);
        switch (opcode) {
            // Single parameter instructions
            case OPCODE::PUSH:
            case OPCODE::ARITH:
            case OPCODE::LOGIC:
            case OPCODE::COMPARE:
            case OPCODE::LOADR:
            case OPCODE::STORER:
                state.parameters_due = 1;
                break;
            // Double parameter instructions
            case OPCODE::BSHIFT:
            case OPCODE::LOADM:
            case OPCODE::STOREM:
            case OPCODE::JMP:
            case OPCODE::IO:
            case OPCODE::FUNCT:
                state.parameters_due = 2;
                break;
        }
    } else return -1;
    return 0;
}

int Parser::parse(){
    while ((current_token = tokenizer->next_token_to_parse()) != NULL){
        assert(current_token != NULL);
        switch (current_token->type) {
            case lex::TOKENS::DIRECTIVE:
                if (deal_with_directives() == -1) return -1;
                break;
            case lex::TOKENS::PLAIN:
                if (deal_with_opcodes() == -1) {
                    // Must be a parameter
                }
                break;
            case lex::TOKENS::NUMBER:
                while (state.parameters_due > 0) {
                    // A switch? The parameter lengths for the instructions vary
                    current_token = tokenizer->next_token_to_parse();
                    --state.parameters_due;
                }
                break;
            case lex::TOKENS::STRING:
                if (state.mode != MODE::DATA) {
                    cerr << "Error at line" << current_token->line << '\n';
                    cerr << "Strings can not appear outside memory section\n";
                    return -1;
                } else {
                    // Deal with string storage
                }
        }
    }
    return 0;
}
