#ifndef PARSER_HPP
#define PARSER_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

namespace parse
{

    enum TOKEN {
        DIRECTIVE_T, OPCODE_T, NUMERIC_T,
        PLAIN_T, STRING_T
    };

    struct Token {
        TOKEN type;
        std::string value = "";
        int line;
    };

    typedef std::vector<TOKEN*> TokenContainer;

    class Tokenizer {
        private:
            const char *source_name;
            std::ifstream source;
            char current_char;
            inline void next();
            int line = 1;
        public:
            TokenContainer* tokenize();
            Tokenizer(const char *source_name): source_name(source_name){
                source.open(source_name);
            }
    };

}

#endif
