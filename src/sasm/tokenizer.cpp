#include "parser.hpp"
#include "ctype.h"
#include <iostream>

using namespace parse;

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
                current_token->line = line;
                current_token->type = TOKEN::DIRECTIVE_T;
                next();
                while (!is_terminal(current_char)) consume();
                push_token(tokens);
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
