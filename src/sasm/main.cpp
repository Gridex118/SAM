#include <iostream>
#include "lexer.hpp"

using namespace std;

int main(int argc, char **argv){
    if (argc != 2){
        cout << "Syntax: sasm [source file]\n";
    } else {
        if (tokenize(argv[1]) == -1){
            cout << "Error, source file does not exist\n";
        };
    }
    return 0;
}
