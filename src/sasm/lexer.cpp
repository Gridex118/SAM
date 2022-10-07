#include "lexer.hpp"
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;
using namespace lex;

inline bool is_terminal(const char character){
    return (character == ' ') || (character == '\n')
            || (character == '\r') || (character == '\t');
}

inline void Tokenizer::next(){
    source >> noskipws >> current_char;
}

Tokenizer::Tokenizer(const char *source_name){
    source.open(source_name);
    next();
}

Tokenizer::Tokenizer(const std::string source_name){
    char *char_array = new char[source_name.length() + 1];
    std::strcpy(char_array, source_name.c_str());
    source.open(char_array);
    next();
}

inline void Tokenizer::add_char_to_token(){
    tokens.concat_to_value(current_char);
    next();
}

inline void Tokenizer::consume_string(){
    next();
    while (current_char != '\"') add_char_to_token();
    next();
}

Token* Tokenizer::next_token_to_parse(){
    return tokens[next_token_index_for_parsing++];
}

int Tokenizer::tokenize(){
    while (source) {
        switch (current_char) {
            case ';':
                while (current_char != '\n') next();
                break;
            case '\"':
                tokens.set_type(TOKENS::STRING);
                tokens.set_line(line);
                consume_string();
                if (tokens.push_token() == -1) return -1;
                break;
            case '\n':
                ++line;
            case '\r':
            case '\t':
            case ' ':
                next();
                break;
            default:
                tokens.set_line(line);
                tokens.set_type(current_char);
                if (tokens.is_type(TOKENS::DIRECTIVE)) next();
                while (!is_terminal(current_char) && source) add_char_to_token();
                if (tokens.push_token() == -1) return -1;
                break;
        }
    }
    return 0;
}
