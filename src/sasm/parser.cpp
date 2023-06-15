#include "parser.hpp"
#include "string.h"
#include <iostream>

using namespace parse;

std::unordered_map<std::string, int> PARAMETER_MAP = {
    {"high", BYTE_ORDERS::HIGH}, {"low", BYTE_ORDERS::LOW},
    {"add", ARITHMETIC_OPERATIONS::ADD}, {"sub", ARITHMETIC_OPERATIONS::SUB},
    {"mult", ARITHMETIC_OPERATIONS::MULT}, {"div", ARITHMETIC_OPERATIONS::DIV},
    {"incr", ARITHMETIC_OPERATIONS::INCR}, {"and", LOGICAL_OPERATIONS::AND},
    {"or", LOGICAL_OPERATIONS::OR}, {"not", LOGICAL_OPERATIONS::NOT},
    {"left", BSHIFT_PARA::LEFT}, {"right", BSHIFT_PARA::RIGHT},
    {"eql", COMPARISON_PARA::EQL}, {"neql", COMPARISON_PARA::NEQL},
    {"ls", COMPARISON_PARA::LS}, {"gr", COMPARISON_PARA::GR},
    {"input", IO_PARA::INPUT}, {"print", IO_PARA::PRINT},
    {"print_eseq", IO_PARA::PRINT_ESEQ}, {"integer", IO_TYPES::INTEGER},
    {"string", IO_TYPES::STRING}, {"char", IO_TYPES::CHAR},
    {"newline", ESCAPE_SEQUENCES::NEWLINE},
    {"return_carriage", ESCAPE_SEQUENCES::RETURN_CARRIAGE},
    {"uncond", JUMP_OPTIONS::UNCOND}, {"cond", JUMP_OPTIONS::COND},
    {"call", FUNCT_OPTIONS::CALL}, {"return", FUNCT_OPTIONS::RETURN},
    {"Ra", REGISTERS::Ra}, {"Rb", REGISTERS::Rb}, {"Rc", REGISTERS::Rc},
    {"Rsp", REGISTERS::Rsp}, {"Rip", REGISTERS::Rip},
    {"Rcom", REGISTERS::Rcom}, {"Rhlt", REGISTERS::Rhlt},
    {"Rerr", REGISTERS::Rerr}, {"Rcbindx", REGISTERS::Rcbindx},
    {"Rvbindx", REGISTERS::Rvbindx}, {"var", MEMORY_SECTIONS::VAR},
    {"code", MEMORY_SECTIONS::CODE}
};


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
                if (PARAMETER_MAP.find(token->str_value) != PARAMETER_MAP.end()) {
                    token->type = NUMERIC_T;
                    token->int_value = PARAMETER_MAP[token->str_value];
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
            case TOKEN::LABEL_DIRECTIVE_T:
                data[token->str_value] = instruction_count - 1;
                break;
            case TOKEN::SECTION_DIRECTIVE_T:
                if (token->str_value == "CODE") {
                    token->int_value = CODE_SECTION_START;
                } else if (token->str_value == "MEM") {
                    token->int_value = MEM_SECTION_START;
                }
                output_sink->push_back(token);
                break;
            case TOKEN::INCLUDE_DIRECTIVE_T:
                {
                    char include_file[1024];
                    strcpy(include_file, (token->str_value).c_str());
                    parse(include_file, output_sink);
                }
                break;
            default:
                break;
        }
    }
}
