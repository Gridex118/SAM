#include "../src/sasm/parser.hpp"
#include <iostream>

int main() {
    std::string test_input = "push 100";
    parse::Token *push = new parse::Token {
        parse::TOKEN::OPCODE_T, "push", 1
    };
    parse::Token *hundred = new parse::Token {
        parse::TOKEN::NUMERIC_T, "100", 1
    };
    parse::TokenContainer *expected = new parse::TokenContainer;
    expected->push_back(push);
    expected->push_back(hundred);
    parse::Tokenizer tokenizer("tests/push_100.sasm");
    parse::TokenContainer *candidate = tokenizer.tokenize();
    for (int i = 0; i < static_cast<int>(expected->size()); ++i){
        parse::Token token_e = *(*expected)[i];
        parse::Token token_c = *(*candidate)[i];
        std::cout << token_e.type << " " << token_c.type << '\n';
        std::cout << token_e.value << " " << token_c.value << '\n';
        std::cout << token_e.line << " " << token_c.line << "\n\n";
    }
    return 0;
}
