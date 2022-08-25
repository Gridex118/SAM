#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"

namespace parse {

    class Parser{
        private:
            std::ofstream sink;
            lex::Tokenizer *tokenizer;
        public:
            Parser(
                const char *sink_name, lex::Tokenizer *tokenizer
            ): tokenizer(tokenizer) { sink.open(sink_name); };
            int parse();
    };

}

#endif
