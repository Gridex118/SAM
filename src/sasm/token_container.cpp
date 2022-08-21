#include "lexer.hpp"

using namespace std;
using namespace lexer;

inline bool is_alpha(char character){
    return (character >= 'A') && (character <= 'z');
}

inline bool is_numeric(char character){
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

inline void TokenContainer::reset_token(){
    current_token = new Token;
}

void TokenContainer::push_token(){
    push_back(current_token);
    reset_token();
}
