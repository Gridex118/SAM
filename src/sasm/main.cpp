#include <iostream>
#include "lexer.hpp"

using namespace std;

int main(int argc, char **argv){
    if ((argc != 2) && (argc != 3)) {
        cout << "Syntax: sasm [source file]\n";
    } else if (argc == 2) {
        lexer::Tokenizer tokenizer(argv[1]);
        if (tokenizer.tokenize() == NULL) {
            cerr << "Failed to tokenize" << '\n';
            return -1;
        }
    } else if (argc == 3) {
        lexer:: Tokenizer tokenizer(argv[1], argv[2]);
        if (tokenizer.tokenize() == NULL) {
            cerr << "Failed to tokenize" << '\n';
            return -1;
        }
    }
    return 0;
}
