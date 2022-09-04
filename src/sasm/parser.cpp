#include "parser.hpp"
#include "../vm/vm.h"
#include <iostream>
#include <assert.h>

using namespace std;
using namespace parse;

vector<uint16_t>* string_to_words(string str){
    vector<uint16_t> *words = new vector<uint16_t>;
    for (unsigned int i = 0; i < str.length(); i += 2) {
        words->push_back(
            (static_cast<uint8_t>(str[i]) << 8)
            + static_cast<uint8_t>(str[i+1])
        );    // Consuming two charcters at a time
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

int match_parameter(const string &candidate){
    return -1;
}

inline int match_directive(const string &candidate){
    if (candidate == "SECTION") return DIRECTIVE::SECTION;
    else if (candidate == "LABEL") return DIRECTIVE::LABEL;
    else return -1;
}

inline void Parser::write(){
    sink << "0x" << hex << instruction << '\n';
    instruction = 0;
}

inline void Parser::write_str_as_bytes(){
    vector<uint16_t> *words = string_to_words(current_token->value);
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
        state.parameters_due = parameters_due_for_opcode(opcode);
        state.second_parameter_size = get_second_parameter_size(opcode);
    } else return -1;
    return 0;
}

int Parser::deal_with_numbers(){
    int number = stoi(current_token->value);
    assert(
        (state.parameters_due == 2)
        || (
            (state.parameters_due < 2)
            && (state.second_parameter_size == 0)
        )
    );
    instruction += (
        number << (
            state.parameters_due == 2 ?
            (MAX_PARAMETER_SIZE - state.second_parameter_size) : 0
        )
    );
    --state.parameters_due;
    if (state.parameters_due == 0) write();
    return 0;
}

int Parser::parse(){
    while ((current_token = tokenizer->next_token_to_parse()) != NULL){
        assert(current_token != NULL);
        switch (current_token->type) {
            case lex::TOKENS::DIRECTIVE:
                if (deal_with_directives() == -1) return -1;
                break;
            case lex::TOKENS::NUMBER:
                if ((state.mode == MODE::CODE) && (state.parameters_due > 0)) {
                    if (deal_with_numbers() == -1) return -1;
                    break;
                } else ; // Fall through to case TOKENS::PLAIN, and report error
            case lex::TOKENS::PLAIN:
                if (state.mode != MODE::CODE) {
                    cerr << "Error at line " << current_token->line << '\n';
                    cerr << "Machine instructions can not appear ";
                    cerr << "outside code section\n";
                    return -1;
                }
                if (deal_with_opcodes() == -1) {
                    if (state.parameters_due == 0){
                        cerr << "Invalid syntax(Line: ";
                        cerr << current_token->line << ")\n";
                        return -1;
                    }

                    --state.parameters_due;
                    if (state.parameters_due == 0) write();
                }
                break;
            case lex::TOKENS::STRING:
                if (state.mode != MODE::DATA) {
                    cerr << "Error at line " << current_token->line << '\n';
                    cerr << "Strings can not appear outside memory section\n";
                    return -1;
                } else {
                    write_str_as_bytes();
                }
        }
    }
    return 0;
}
