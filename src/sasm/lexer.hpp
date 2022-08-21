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

    class Tokenizer;

    class TokenContainer : public std::vector<Token*>{
        private:
            Token *current_token = new Token;
        protected:
            inline void reset_token();
        public:
            void push_token();
            bool is_type(TOKENS type);
            void set_type(TOKENS type);
            void set_type(char character);
            void set_line(int line);
            void concat_to_value(char character);
    };

    class Tokenizer{
        private:
            std::ifstream source;
            TokenContainer tokens;
            char current_char;
            int line = 1;
            inline void next();
            inline void add_char_to_token();
        public:
            Tokenizer(char *source_name);
            int tokenize();
    };


}

#endif
