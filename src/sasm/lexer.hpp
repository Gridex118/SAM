#ifndef LEXER_H
#define LEXER_H

extern "C"{
    #include "../vm/vm.h"
}

#include <vector>
#include <string>
#include <fstream>

namespace lexer{

    struct Token{
        int type;
        std::string value;
    };

    typedef std::vector<Token> TokenContainer;

    class Tokenizer{
        private:
            std::ifstream source;
            std::ofstream sink;
        public:
            Tokenizer(char *source_name){
                source.open(source_name);
                sink.open("a.txt");
            }
            Tokenizer(char *source_name, char *sink_name){
                source.open(source_name);
                sink.open(sink_name);
            }
            TokenContainer* tokenize();
    };


}

#endif
