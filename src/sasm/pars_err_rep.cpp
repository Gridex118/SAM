#include "parser.hpp"
#include <iostream>


void parse::report_string_outside_data_sec_error(int line){
    std::cerr << "Error at line " << line << '\n';
    std::cerr << "Strings can not appear outside memory section\n";
}

void parse::report_instr_outside_code_sec_error(int line){
    std::cerr << "Error at line " << line << '\n';
    std::cerr << "Machine instructions can not appear ";
    std::cerr << "outside code section\n";
}

void parse::report_syntax_error(int line){
    std::cerr << "Invalid syntax(Line: ";
    std::cerr << line << ")\n";
}

void parse::report_directive_error(int line){
    std::cerr << "Illegal directive at line " << line << '\n'; 
}

void parse::report_section_error(int line){
    std::cerr << "Illegal section specifier at line " << line << '\n';
}

void parse::report_parameter_error(int line){
    std::cerr << "Illegal parameter passed at line " << line << '\n';
}
