#include "parser.hpp"
#include "../vm/vm.h"

using namespace std;
using namespace parse;

int match_opcode(string &candidate){
    return -1;
}

int Parser::parse(){
    lex::Token* current_token;
    int instruction;
    while ((current_token = tokenizer->next_token()) != NULL){
        if (match_opcode(current_token->value) == -1);
    }
    return 0;
}
