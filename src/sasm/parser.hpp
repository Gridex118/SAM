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

    struct IteratorFileNameTuple{
        TokenContainer::iterator iterator;
        TokenContainer::iterator end;
        char *source_file_name;
    };

    using ParserOutput = Token;

    typedef std::vector<ParserOutput*> ParserOutputContainer;

    class Parser {
        private:
            char *current_source_file_name;
            int instruction_count = 0;
            std::unordered_map<std::string, int> data;
            std::vector<IteratorFileNameTuple*> history;
            IteratorFileNameTuple *current_ifnt;
            inline int push_new_ifnt();
            ParserOutputContainer* parse(char *file_name);
            ParserOutputContainer* replace_labels(ParserOutputContainer *raw_output);
        public:
            ParserOutputContainer* parse();
            Parser(char *source_name);
    };

}

#endif
