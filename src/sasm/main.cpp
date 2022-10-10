#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char **argv){
    if ((argc != 2) && (argc != 3)) {
        std::cout << "Syntax: sasm SOURCE [SINK] \n";
    } else {
        parse::Parser parser(
            (argc == 3) ? argv[2] : "a.sam", argv[1]
        );
        if (parser.parse() == -1) {
            std::cerr << "Error encountered while parsing\n";
            return -1;
        }
    }
    return 0;
}
