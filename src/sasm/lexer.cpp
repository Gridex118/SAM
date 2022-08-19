#include "lexer.hpp"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace lexer;

bool is_alpha(char character){
    return (character >= 'A') && (character <= 'z');
}

bool is_terminal(char character){
    return (character == ' ') || (character == '\n') (character == EOF);
}

bool is_numeric(char character){
    return (character >= '0') && (character <= '9');
}

ostream& operator<<(ostream &os, Token &token){
    os << token.value << " " << token.type;
    return os;
}

void Tokenizer::next(){
    current_char = source.get();
}

Tokenizer::Tokenizer(char *source_name){
    source.open(source_name);
    next();
}

int Tokenizer::tokenize(){
    current_token = new Token;
    while (current_char != EOF) {
        switch (current_char) {
            case ';':
                while (current_char != '\n') next();
                break;
            case '\n':
                ++line;
            case ' ':
                current_token = new Token;
                next();
                break;
            default:
                current_token->line = line;
                if (is_alpha(current_char)) {
                    current_token->type = TOKENS::STRING;
                } else if (is_numeric(current_char)) {
                    current_token->type = TOKENS::NUMBER;
                } else if (current_char == '.') {
                    current_token->type = TOKENS::LABEL;
                    next();    // Skip the dot
                }
                while (!is_terminal(current_char)) {
                    cout << (int) current_char << '\n';
                    current_token->value += current_char;
                    next();
                }
                tokens.push_back(current_token);
                break;
        }
    }
    // for (auto i = tokens.begin(); i < tokens.end(); ++i){
    //     cout << **i << '\n';
    // }
    return 0;
}
