#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include "../vm/vm.h"

namespace parse
{

    enum TOKEN {
        DIRECTIVE_T, OPCODE_T, NUMERIC_T,
        PLAIN_T, STRING_T
    };

    struct Token {
        TOKEN type;
        std::string str_value = "";
        int int_value = 0;
        int line;
    };

    typedef std::vector<Token*> TokenContainer;

    extern std::unordered_map<std::string, OPCODE> OPCODE_MAP;

    class Tokenizer {
        private:
            std::ifstream source;
            char current_char;
            Token *current_token;
            inline void push_token(TokenContainer *container);
            inline void next();
            inline void consume();
            int line = 1;
        public:
            TokenContainer* tokenize();
            Tokenizer(const char *source_name);
    };

    using ParserOutput = Token;

    typedef std::vector<ParserOutput*> ParserOutputContainer;

    class Parser {
        private:
            char *current_source_file_name;
            int instruction_count = 0;
			struct {
				unsigned int expecting_label : 1;
				unsigned int expecting_include : 1;
			} flags;
            std::unordered_map<std::string, int> data;
            void parse(char *file_name, ParserOutputContainer*& output_sink);
            void replace_labels(ParserOutputContainer *raw_output);
        public:
            ParserOutputContainer* parse();
            Parser(char *source_name);
    };

}

#endif
