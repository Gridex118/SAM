#include "../src/sasm/parser.hpp"
#include <sstream>
#include <iostream>

int main() {
    std::string test_input = "push 100";
    std::stringstream test_input_stream(test_input);
    parse::Token *push, *hundred;
    push->type = parse::TOKEN::OPCODE_T;
    push->line = 1;
    push->value = "push";
    hundred->type = parse::TOKEN::NUMERIC_T;
    hundred->line = 1;
    hundred->value = "100";
    parse::TokenContainer *expected;
    expected->push_back(push);
    expected->push_back(hundred);
    parse::Tokenizer<std::stringstream> tokenizer(test_input_stream);
    if (!(tokenizer.tokenize() == expected)) {
        std::cerr << "Test Failed\n";
    }
}
