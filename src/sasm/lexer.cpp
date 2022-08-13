#include "lexer.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int tokenize(char *file_name){
    ifstream source(file_name);
    if (!(source.good())) return -1;
    char current;
    while ((current = source.get()) != EOF) {
        cout << current << '\n';
    }
    source.close();
    return 0;
}
