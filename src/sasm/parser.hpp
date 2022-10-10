#ifndef PARSER_H
#define PARSER_H

#include "lexer.hpp"
#include <cstdint>
#include <unordered_map>

namespace parse {

    enum MODE { CODE, DATA };

    void report_string_outside_data_sec_error(int line);
    void report_instr_outside_code_sec_error(int line);
    void report_syntax_error(int line);
    void report_directive_error(int line);
    void report_section_error(int line);
    void report_parameter_error(int line);

    typedef struct parser_state {
        int instruction_count = 0;
        unsigned short current_opcode: 4;
        unsigned short parameters_due: 2;
        unsigned short second_parameter_size: 4;
        MODE mode: 1;
        parser_state() {
            parameters_due = 0;
            second_parameter_size = 0;
            mode = MODE::CODE;
        }
    } ParserState;

    class Parser{
        private:
            std::ofstream sink;
            lex::Tokenizer *primary_tokenizer;
            lex::Tokenizer *secondary_tokenizer;
            std::vector<lex::Token*> tokens_final;
            lex::Token *current_token;
            uint16_t instruction = 0;
            std::unordered_map<std::string, int> data;
            ParserState state;
            int populate_tokens_final();
            int deal_with_directives();
            int deal_with_opcodes();
            int deal_with_num_parameters();
            int deal_with_plain_parameters();
            int deal_with_imports();
            inline int add_para_to_instr();
            inline void write();
            inline void write_str_as_bytes();
        public:
            Parser(const char *sink_name, const char *source_name){
                sink.open(sink_name);
                primary_tokenizer = new lex::Tokenizer(source_name);
            }
            int parse();
    };

}

#endif
