#include "../src/sasm/parser.hpp"
#include <iostream>

int main() {
    parse::Parser parser("tests/push_100_parsing.sasm");
	parse::ParserOutputContainer *outputs = parser.parse();
	for (auto output=outputs->begin(); output != outputs->end(); ++output) {
		std::cout << (*output)->str_value << ' ';
		std::cout << (*output)->int_value << '\n';
	}
	return 0;
}
