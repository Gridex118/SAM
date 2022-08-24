#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

using namespace std;

int main(int argc, char **argv){
    if ((argc != 2) && (argc != 3)) {
        cout << "Syntax: sasm SOURCE [SINK] \n";
    } else {
        lexer::Tokenizer tokenizer(argv[1]);
        if (tokenizer.tokenize() == -1) {
            cerr << "Error encountered at line" << tokenizer.line;
            cerr << "While tokenizing\n";
        };
        parser::Parser parser(
            (argc == 3) ? argv[2] : "a.txt", &tokenizer
        );
    }
    return 0;
}
