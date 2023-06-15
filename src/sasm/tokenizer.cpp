#include "parser.hpp"
#include "ctype.h"
#include <iostream>

using namespace parse;

std::unordered_map<std::string, OPCODE> parse::OPCODE_MAP = {
    {"push", OPCODE::PUSH}, {"pop", OPCODE::POP}, {"dup", OPCODE::DUP},
    {"arith", OPCODE::ARITH}, {"bshift", OPCODE::BSHIFT}, {"logic", OPCODE::LOGIC},
    {"compare", OPCODE::COMPARE}, {"loadr", OPCODE::LOADR}, {"loadm", OPCODE::LOADM},
    {"storer", OPCODE::STORER}, {"storem", OPCODE::STOREM}, {"jmp", OPCODE::JMP},
    {"io", OPCODE::IO}, {"funct", OPCODE::FUNCT}, {"halt", OPCODE::HALT}
};

inline TOKEN directive_type(std::string candidate) {
    if (candidate == "SECTION") return TOKEN::SECTION_DIRECTIVE_T;
    else if (candidate == "LABEL") return TOKEN::LABEL_DIRECTIVE_T;
    else if (candidate  == "INCLUDE") return TOKEN::INCLUDE_DIRECTIVE_T;
    else return TOKEN::NO_MATCH;
}

inline bool is_opcode(const std::string &candidate) {
    return (OPCODE_MAP.find(candidate) != OPCODE_MAP.end());
}

inline bool str_is_num(const std::string &candidate) {
    auto i = candidate.begin();
    while ((i != candidate.end()) && (std::isdigit(*i))) ++i;
    return (i == candidate.end());
}

inline bool is_terminal(const char &candidate) {
    return (
        (candidate == '\n') || (candidate == ' ')
        || (candidate == '\t') || (candidate == '\r')
    );
}

Tokenizer::Tokenizer(const char *source_name) {
    source.open(source_name);
    next();
    current_token = new Token;
}

inline void Tokenizer::next() {
    source >> std::noskipws >> current_char;
}

inline void Tokenizer::consume() {
    current_token->str_value += current_char;
    next();
}

inline void Tokenizer::push_token(TokenContainer *container) {
    container->push_back(current_token);
    current_token = new Token;
}

TokenContainer* Tokenizer::tokenize() {
    TokenContainer *tokens = new TokenContainer;
    while (source) {
        switch (current_char) {
            case ';':
                // Marks a comment; skip the remaining line
                while ((current_char != '\n') && source) next();
                break;
            case '"':
                current_token->line = line;
                current_token->type = TOKEN::STRING_T;
                next();
                while (current_char != '"') consume();
                push_token(tokens);
                next();    // Skip the last "
                break;
            case '.':
                {
                    std::string directive_name;
                    current_token->line = line;
                    next();
                    while (!is_terminal(current_char)) {
                        directive_name += current_char;
                        next();
                    }
                    while (is_terminal(current_char)) next();
                    current_token->type = directive_type(directive_name);
                    while (!is_terminal(current_char)) consume();
                    push_token(tokens);
                }
                break;
            case '\n':
                ++line;
            case '\r': case '\t': case ' ':
                next();
                break;
            default:
                current_token->line = line;
                while (!is_terminal(current_char)) consume();
                if (str_is_num(current_token->str_value)) {
                    current_token->type = TOKEN::NUMERIC_T;
                    current_token->int_value = std::stoi(current_token->str_value);
                } else {
                    if (is_opcode(current_token->str_value))
                        current_token->type = TOKEN::OPCODE_T;
                    else
                        current_token->type = TOKEN::PLAIN_T;
                }
                push_token(tokens);
                break;
        }
    }
    return tokens;
}
