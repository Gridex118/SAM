#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <fstream>

namespace lexer{

    enum TOKENS{
        STRING, NUMBER, LABEL
    };

    struct Token{
        TOKENS type;
        std::string value;
        int line;
        Token(){ value = ""; }
    };

    typedef std::vector<Token*> TokenContainer;

    class Tokenizer{
        private:
            std::ifstream source;
            TokenContainer tokens;
            Token *current_token;
            char current_char;
            int line;
            void next();
        public:
            Tokenizer(char *source_name);
            TokenContainer* tokenize();
    };


}

#endif
