#ifndef LEXER_H
#define LEXER_H

extern "C"{
    #include "../vm/vm.h"
    enum OPCODE;
}

class Token{
    public:
        char token;
};

int tokenize(char *file_name);

#endif
