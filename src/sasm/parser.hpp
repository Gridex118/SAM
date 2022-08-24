#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"
namespace parser {
    class Parser{
        private:
            std::ofstream sink;
            lexer::Tokenizer *tokenizer;
        public:
            Parser(
                const char *sink_name, lexer::Tokenizer *tokenizer
            ): tokenizer(tokenizer) { sink.open(sink_name); };
            int parse();
    };
}
#endif
