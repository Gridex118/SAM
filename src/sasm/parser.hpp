#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"
#include <cstdint>
#include <unordered_map>

namespace parse {

    enum MODE { CODE, DATA };

    class Parser;

    inline int match_section(const string&, Parser&);

    class Parser{
        friend int match_section();
        private:
            std::ofstream sink;
            lex::Tokenizer *tokenizer;
            lex::Token *current_token;
            uint16_t instruction = 0;
            std::unordered_map<std::string, int> data;
            struct State {
                unsigned short current_opcode: 4;
                unsigned short parameters_due: 2;
                MODE mode: 1;
                State() {
                    parameters_due = 0;
                    mode = MODE::CODE;
                }
            } state;
            int deal_with_directives();
            int deal_with_opcodes();
        public:
            Parser(
                const char *sink_name, lex::Tokenizer *tokenizer
            ): tokenizer(tokenizer) { sink.open(sink_name); }
            int parse();
    };

}

#endif
