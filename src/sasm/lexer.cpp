#include "lexer.hpp"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace lexer;

bool is_alpha(char character){
    return (character >= 'A') && (character <= 'z');
}

bool is_terminal(char character){
    return (character == ' ') || (character == '\n')
            || (character == '\r');
}

bool is_numeric(char character){
    return (character >= '0') && (character <= '9');
}

void Tokenizer::next(){
    source >> noskipws >> current_char;
}

Tokenizer::Tokenizer(char *source_name){
    source.open(source_name);
    next();
}

void Tokenizer::add_char_to_token(){
    current_token->value += current_char;
    next();
}

int Tokenizer::tokenize(){
    current_token = new Token;
    while (source) {
        switch (current_char) {
            case ';':
                while (current_char != '\n') next();
                break;
            case '\"':
                current_token->type = TOKENS::STRING;
                current_token->line = line;
                next();
                while (current_char != '\"') add_char_to_token();
                next();
                break;
            case '\n':
                ++line;
            case '\r':
            case ' ':
                tokens.push_back(current_token);
                current_token = new Token;
                next();
                break;
            default:
                current_token->line = line;
                if (is_alpha(current_char)) {
                    current_token->type = TOKENS::PLAIN;
                } else if (is_numeric(current_char)) {
                    current_token->type = TOKENS::NUMBER;
                } else if (current_char == '.') {
                    current_token->type = TOKENS::DIRECTIVE;
                    next();    // Skip the dot
                }
                while (!is_terminal(current_char) && source) add_char_to_token();
                break;
        }
    }
    return 0;
}
