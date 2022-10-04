#include "lexer.hpp"
#include <iostream>

using namespace lex;

inline bool is_alpha(const char character){
    return (character >= 'A') && (character <= 'z');
}

inline bool is_numeric(const char character){
    return (character >= '0') && (character <= '9');
}

bool TokenContainer::is_type(TOKENS type){
    return (current_token->type == type);
}

void TokenContainer::set_line(int line){
    current_token->line = line;
}

void TokenContainer::set_type(TOKENS type){
    current_token->type = type;
}

void TokenContainer::concat_to_value(char character){
    current_token->value += character;
}

void TokenContainer::set_type(char character){
    if (is_alpha(character)) {
        set_type(TOKENS::PLAIN);
    } else if (is_numeric(character)) {
        set_type(TOKENS::NUMBER);
    } else if (character == '.') {
        set_type(TOKENS::DIRECTIVE);
    }
}

inline int TokenContainer::reset_token(){
    current_token = new Token;
    if (current_token == NULL) {
        std::cerr << "Ran out of memory\n";
        return -1;
    }
    else return 0;
}

int TokenContainer::push_token(){
    tokens.push_back(current_token);
    return reset_token();
}

Token* TokenContainer::operator[](unsigned int index){
    if (index >= tokens.size()) return NULL;
    return tokens[index];
}
