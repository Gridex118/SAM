#include "parser.hpp"
#include "ctype.h"

using namespace parse;

inline void Tokenizer::next() {
    source >> std::noskipws >> current_char;
}

inline bool str_is_alpha(std::string &candidate) {
    auto i = candidate.begin();
    for (i; (i != candidate.end()) && (std::isalpha(*i)); ++i);
    return (i == candidate.end());
}

inline bool str_is_num(std::string &candidate) {
    auto i = candidate.begin();
    for (i; (i != candidate.end()) && (std::isdigit(*i)); ++i);
    return (i == candidate.end());
}

TokenContainer* Tokenizer::tokenize() {
    next();
    TokenContainer *tokens = new TokenContainer;
    Token *token = new Token;
    while (source) {
        switch (current_char) {
            case ';':
                while (current_char != '\n') next();
                break;
            case '"':
                token->type = TOKEN::STRING_T;
                token->line = line;
                next();
                while (current_char != '"') {
                    token->value += current_char;
                    next();
                }
                next();
                break;
            case '\n':
                ++line;
            case '\r': case '\t': case ' ':
                next();
                break;
            default:
                break;
        }
    }
    return tokens;
}
