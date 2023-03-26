#include "parser.hpp"
#include "string.h"
#include <iostream>

using namespace parse;

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

inline DIRECTIVE directive_type(std::string candidate) {
    if (candidate == "SECTION") return DIRECTIVE::SECTION;
    else if (candidate == "LABEL") return DIRECTIVE::LABEL;
    else if (candidate  == "INCLUDE") return DIRECTIVE::INCLUDE;
    else return DIRECTIVE::NO_MATCH;
}

Parser::Parser(char *source_name) {
    current_source_file_name = source_name;
}

ParserOutputContainer* Parser::parse() {
    ParserOutputContainer *outputs = new ParserOutputContainer;
    parse(current_source_file_name, outputs);
    replace_labels(outputs);
    return outputs;
}

void Parser::replace_labels(ParserOutputContainer* raw_output) {
    for (auto token_iterator = raw_output->begin(); token_iterator != raw_output->end(); ++token_iterator) {
        Token *token = *token_iterator;
        if (token->type == TOKEN::PLAIN_T) {
            if (data.find(token->str_value) != data.end()) {
                // if the string value is found, it must be a label, and its value should, thus, be substituted with the instruction index
                token->type = TOKEN::NUMERIC_T;
                token->int_value = data[token->str_value];
                token->str_value = "";
            }
        }
    }
}

void Parser::parse(char *file_name, ParserOutputContainer*& output_sink) {
    Tokenizer tokenizer(file_name);
    TokenContainer *tokens = tokenizer.tokenize();
    for (auto token_iterator = tokens->begin(); token_iterator != tokens->end(); ++token_iterator) {
        Token *token = *token_iterator;
        switch (token->type) {
            case TOKEN::PLAIN_T:
                if (flags.expecting_label == 1) {
                    flags.expecting_label = 0;
                    data[token->str_value] = instruction_count - 1;
                    break;
                } else if (flags.expecting_include == 1) {
                    flags.expecting_include = 0;
                    char include_file[1024];
                    strcpy(include_file, (token->str_value).c_str());
                    parse(include_file, output_sink);
                    break;
                } else {
                    token->int_value = match_parameter(token->str_value);
                }
            case TOKEN::STRING_T: case TOKEN::NUMERIC_T:
                // If its a PLAIN(and neither the label nor the include flag is set to 1), STRING, NUMERIC just add it to outputs
                output_sink->push_back(token);
                break;
            case TOKEN::OPCODE_T:
                // If its an OPCODE, increment the instruction counter, and add the token to outputs
                token->int_value = static_cast<int>(OPCODE_MAP[token->str_value]);
                ++instruction_count;
                output_sink->push_back(token);
                break;
            case TOKEN::DIRECTIVE_T:
                switch (directive_type((token)->str_value)) {
                    case DIRECTIVE::LABEL:
                        // If its a LABEL DIRECTIVE, set label flag to 1; the next (plain) token will be used as a label name
                        flags.expecting_label = 1;
                        break;
                    case DIRECTIVE::SECTION:
                        // If its a SECTION DIRECTIVE, add it to outputs
                        output_sink->push_back(token);
                        break;
                    case DIRECTIVE::INCLUDE:
                        flags.expecting_include = 1;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }
}
