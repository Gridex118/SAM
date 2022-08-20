#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <fstream>

namespace lexer{

    enum TOKENS{
        STRING, NUMBER, DIRECTIVE, PLAIN
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
            int line = 1;
            void next();
            void add_char_to_token();
        public:
            Tokenizer(char *source_name);
            int tokenize();
    };


}

#endif
