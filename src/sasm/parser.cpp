#include "parser.hpp"
#include "string.h"
#include <iostream>

using namespace parse;

inline DIRECTIVE directive_type(std::string candidate) {
    if (candidate == "SECTION") return DIRECTIVE::SECTION;
    else if (candidate == "LABEL") return DIRECTIVE::LABEL;
    else if (candidate  == "INCLUDE") return DIRECTIVE::INCLUDE;
    else return DIRECTIVE::NO_MATCH;
}

Parser::Parser(char *source_name) {
    current_source_file_name = source_name;
}

ParserOutputContainer* Parser::parse() {
    ParserOutputContainer *outputs = new ParserOutputContainer;
    parse(current_source_file_name, outputs);
    replace_labels(outputs);
    return outputs;
}

void Parser::replace_labels(ParserOutputContainer* raw_output) {
    for (auto token_iterator = raw_output->begin(); token_iterator != raw_output->end(); ++token_iterator) {
        Token *token = *token_iterator;
        if (token->type == TOKEN::PLAIN_T) {
            if (data.find(token->str_value) != data.end()) {
                // if the string value is found, it must be a label, and its value should, thus, be substituted with the instruction index
                token->type = TOKEN::NUMERIC_T;
                token->int_value = data[token->str_value];
                token->str_value = "";
            }
        }
    }
}

void Parser::parse(char *file_name, ParserOutputContainer*& output_sink) {
    Tokenizer tokenizer(file_name);
    TokenContainer *tokens = tokenizer.tokenize();
    for (auto token_iterator = tokens->begin(); token_iterator != tokens->end(); ++token_iterator) {
        Token *token = *token_iterator;
        switch (token->type) {
            case TOKEN::PLAIN_T:
                {
                    if (flags.expecting_label == 1) {
                        flags.expecting_label = 0;
                        break;
                    } else if (flags.expecting_include == 1) {
                        flags.expecting_include = 0;
                        char include_file[1024];
                        strcpy(include_file, (token->str_value).c_str());
                        parse(include_file, output_sink);
                        break;
                    }
                }
            case TOKEN::STRING_T: case TOKEN::NUMERIC_T:
                // If its a PLAIN(and neither the label nor the include flag is set to 1), STRING, NUMERIC just add it to outputs
                output_sink->push_back(token);
                break;
            case TOKEN::OPCODE_T:
                // If its an OPCODE, increment the instruction counter, and add the token to outputs
                ++instruction_count;
                output_sink->push_back(token);
                break;
            case TOKEN::DIRECTIVE_T:
                switch (directive_type((token)->str_value)) {
                    case DIRECTIVE::LABEL:
                        // If its a LABEL DIRECTIVE, set label flag to 1; the next (plain) token will be used as a label name
                        flags.expecting_label = 1;
                        break;
                    case DIRECTIVE::SECTION:
                        // If its a SECTION DIRECTIVE, add it to outputs
                        output_sink->push_back(token);
                        break;
                    case DIRECTIVE::INCLUDE:
                        flags.expecting_include = 1;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }
}
