#include "parser.hpp"
#include "string.h"
#include <iostream>

using namespace parse;

inline DIRECTIVE directive_type(std::string candidate) {
    std::string first_word = "";
    for (int i=0; candidate[i] != ' '; ++i) {
        first_word += candidate[i];
    }
    if (first_word == "SECTION") return DIRECTIVE::SECTION;
    else if (first_word == "LABEL") return DIRECTIVE::LABEL;
    else if (first_word == "INCLUDE") return DIRECTIVE::INCLUDE;
    else return DIRECTIVE::NO_MATCH;
}

inline std::string directive_value(std::string candidate) {
    // Skip the first word(which specifies the type), demarcated by the space.
    int i=0;
    for (i; candidate[i] != ' '; ++i);
    return candidate.substr(i+1);   // +1 to skip the space
}

inline int Parser::push_new_ifnt() {
    // Get the tokens for the current source file
    Tokenizer tokenizer(current_source_file_name);
    TokenContainer *tokens = tokenizer.tokenize();
    // Make an IteratorFileNameTuple, and add it to the history
    IteratorFileNameTuple *new_ifnt = new IteratorFileNameTuple;
    if (new_ifnt == NULL) return -1;
    new_ifnt->iterator = tokens->begin();
    new_ifnt->end = tokens->end();
    new_ifnt->source_file_name = current_source_file_name;
    history.push_back(new_ifnt);
    // Set the new ifnt as current
    current_ifnt = new_ifnt;
    return 0;
}

Parser::Parser(char *source_name) {
    current_source_file_name = source_name;
}

ParserOutputContainer* Parser::parse() {
    return parse(current_source_file_name);
}

ParserOutputContainer* Parser::parse(char *file_name) {
    current_source_file_name = file_name;
    ParserOutputContainer *outputs = new ParserOutputContainer;
    if (push_new_ifnt() == -1) return NULL;
    // Iterate throught the tokens
    for (auto token_iterator = current_ifnt->iterator; token_iterator != current_ifnt->end; ++token_iterator) {
        Token *token = *token_iterator;
        switch (token->type) {
            case TOKEN::PLAIN_T: case TOKEN::STRING_T: case TOKEN::NUMERIC_T:
                // If its a PLAIN, STRING, NUMERIC just add it to outputs
                outputs->push_back(token);
                break;
            case TOKEN::OPCODE_T:
                // If its an OPCODE, increment the instruction counter, and add the token to outputs
                ++instruction_count;
                outputs->push_back(token);
                break;
            case TOKEN::DIRECTIVE_T:
                switch (directive_type((token)->str_value)) {
                    case DIRECTIVE::LABEL:
                        // If its a LABEL DIRECTIVE, add (label name : current instruction count) to the data map
                        data[directive_value(token->str_value)] = instruction_count + 1; // +1 since the vm increments the ip for every instruction
                        break;
                    case DIRECTIVE::SECTION:
                        // If its a SECTION DIRECTIVE, add it to outputs
                        outputs->push_back(token);
                        break;
                    case DIRECTIVE::INCLUDE:
                        // If its an INCLUDE DIRECTIVE, call parse, and add the included tokens to output
                        {
                            char file_name[1024];
                            strcpy(file_name, directive_value(token->str_value).c_str());
                            ParserOutputContainer *included_outputs = parse(file_name);
                            if (included_outputs == NULL) return NULL;
                            for (auto output = included_outputs->begin(); output != included_outputs->end(); ++output)
                                outputs->push_back(*output);
                        }
                        // since parse adds an IFNT to the history, pop it off
                        history.pop_back();
                        break;
                    default:
                        return NULL;
                        break;
                }
            default:
                return NULL;
                break;
        }
    }
    return outputs;
}
