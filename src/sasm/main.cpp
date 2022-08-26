#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

using namespace std;

int main(int argc, char **argv){
    if ((argc != 2) && (argc != 3)) {
        cout << "Syntax: sasm SOURCE [SINK] \n";
    } else {
        lex::Tokenizer tokenizer(argv[1]);
        if (tokenizer.tokenize() == -1) {
            cerr << "Error encountered at line" << tokenizer.line;
            cerr << "While tokenizing\n";
            return -1;
        };
        parse::Parser parser(
            (argc == 3) ? argv[2] : "a.txt",
            &tokenizer
        );
        if (parser.parse() == -1) {
            cerr << "Error encountered while parsing\n";
            return -1;
        }
    }
    return 0;
}
