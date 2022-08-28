#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"
#include <cstdint>
#include <unordered_map>

namespace parse {

    class Parser{
        private:
            std::ofstream sink;
            lex::Tokenizer *tokenizer;
            lex::Token *current_token;
            uint16_t instruction = 0;
            std::unordered_map<std::string, int> data;
            int deal_with_directives();
            int deal_with_opcodes();
        public:
            Parser(
                const char *sink_name, lex::Tokenizer *tokenizer
            ): tokenizer(tokenizer) { sink.open(sink_name); };
            int parse();
    };

}

#endif
