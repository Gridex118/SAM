#include "../src/sasm/parser.hpp"
#include <iostream>

int main() {
    std::string test_input = "push 100";
    parse::Token *push, *hundred;
    push->type = parse::TOKEN::OPCODE_T;
    push->line = 1;
    push->value = "push";
    hundred->type = parse::TOKEN::NUMERIC_T;
    hundred->line = 1;
    hundred->value = "100";
    parse::TokenContainer *expected = new parse::TokenContainer;
    expected->push_back(push);
    expected->push_back(hundred);
    parse::Tokenizer tokenizer("push_100.sasm");
}
