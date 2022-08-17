#include <iostream>
#include "lexer.hpp"

using namespace std;

int main(int argc, char **argv){
    if ((argc != 2) && (argc != 3)) {
        cout << "Syntax: sasm [source file]\n";
    } else {
        lexer::Tokenizer tokenizer(argv[1]);
        lexer::TokenContainer *tokens;
        if ((tokens = tokenizer.tokenize()) == NULL) return -1;
    }
    return 0;
}
