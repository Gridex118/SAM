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
	ParserOutputContainer *outputs = parse(current_source_file_name);
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

ParserOutputContainer* Parser::parse(char *file_name) {
    current_source_file_name = file_name;
    ParserOutputContainer *outputs = new ParserOutputContainer;
    if (push_new_ifnt() == -1) return NULL;
    for (auto token_iterator = current_ifnt->iterator; token_iterator != current_ifnt->end; ++token_iterator) {
        Token *token = *token_iterator;
        switch (token->type) {
            case TOKEN::PLAIN_T: 
				if (flags.label == 1) {
					data[token->str_value] = instruction_count;
					flags.label = 0;
					break;
				} else if (flags.include == 1) {
					char file_name[1024];
					strcpy(file_name, (token->str_value).c_str());
					ParserOutputContainer *included_outputs = parse(file_name);
					if (included_outputs == NULL) return NULL;
					for (auto output = included_outputs->begin(); output != included_outputs->end(); ++output)
						outputs->push_back(*output);
					flags.include = 0;
					history.pop_back();
					current_ifnt = history[history.size() - 1];
					break;
				}
			case TOKEN::STRING_T: case TOKEN::NUMERIC_T:
                // If its a PLAIN(and neither the label nor the include flag is not set to 1), STRING, NUMERIC just add it to outputs
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
                        // If its a LABEL DIRECTIVE, set label flag to 1; the next (plain) token will be used as a label name
						flags.label = 1;
                        break;
                    case DIRECTIVE::SECTION:
                        // If its a SECTION DIRECTIVE, add it to outputs
                        outputs->push_back(token);
                        break;
                    case DIRECTIVE::INCLUDE:
						flags.include = 1;
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
