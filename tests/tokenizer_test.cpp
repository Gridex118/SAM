#include "../src/sasm/parser.hpp"
#include <iostream>

inline bool operator==(parse::Token &left, parse::Token &right) {
    return (
        (left.type == right.type)
        && (left.value == right.value)
        && (left.line == right.line)
    );
}

inline bool operator!=(parse::Token &left, parse::Token &right) {
    return !(left == right);
}

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
    if (candidate->size() != expected->size()) {
        std::cerr << "Test Failed\n";
            return -1;
    }
    for (int i = 0; i < static_cast<int>(expected->size()); ++i){
        parse::Token token_e = *(*expected)[i];
        parse::Token token_c = *(*candidate)[i];
        if (token_e != token_c) {
            std::cerr << "Test Falied\n";
            return -1;
        }
    }
    std::cout << "Test Passed\n";
    return 0;
}
