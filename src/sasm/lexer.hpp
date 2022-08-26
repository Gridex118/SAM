#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <fstream>

namespace lex{

    enum TOKENS{
        STRING, NUMBER, DIRECTIVE, PLAIN
    };

    struct Token{
        TOKENS type;
        std::string value = "";
        int line;
    };

    class TokenContainer{
        private:
            std::vector<Token*> tokens;
            Token *current_token = new Token;
            inline int reset_token();
        public:
            int push_token();
            bool is_type(TOKENS type);
            void set_type(TOKENS type);
            void set_type(char character);
            void set_line(int line);
            void concat_to_value(char character);
            Token* operator[](unsigned int index);
    };

    class Tokenizer{
        private:
            std::ifstream source;
            TokenContainer tokens;
            char current_char;
            int next_token_index_for_parsing = 0;
            inline void next();
            inline void add_char_to_token();
            inline void consume_string();
        public:
            int line = 1;
            Tokenizer(const char *source_name);
            int tokenize();
            Token* next_token_to_parse();
    };

}

#endif
