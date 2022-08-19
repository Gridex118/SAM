#include <iostream>
#include "lexer.hpp"

using namespace std;

int main(int argc, char **argv){
    if ((argc != 2) && (argc != 3)) {
        cout << "Syntax: sasm SOURCE [SINK] \n";
    } else {
        lexer::Tokenizer tokenizer(argv[1]);
        if (tokenizer.tokenize() == -1) return -1;
    }
    return 0;
}
