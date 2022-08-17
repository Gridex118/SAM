#include "lexer.hpp"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace lexer;

char* Tokenizer::next(){
    current_char = source.get();
    return &current_char;
}

TokenContainer* Tokenizer::tokenize(){
    current_token = new Token;
    while (*(this->next()) != EOF) {
        switch (current_char) {
            case ';':
                while (current_char != '\n') this->next();
                break;
            case ' ':
            case '\n':
                current_token = new Token;
                break;
        }
    }
    return (TokenContainer*) NULL;
}
