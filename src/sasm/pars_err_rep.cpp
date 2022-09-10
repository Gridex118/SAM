#include "parser.hpp"
#include <iostream>

using std::cerr;

void parse::report_string_outside_data_sec_error(int line){
    cerr << "Error at line " << line << '\n';
    cerr << "Strings can not appear outside memory section\n";
}

void parse::report_instr_outside_code_sec_error(int line){
    cerr << "Error at line " << line << '\n';
    cerr << "Machine instructions can not appear ";
    cerr << "outside code section\n";
}

void parse::report_syntax_error(int line){
    cerr << "Invalid syntax(Line: ";
    cerr << line << ")\n";
}

void parse::report_directive_error(int line){
    cerr << "Illegal directive at line " << line << '\n'; 
}

void parse::report_section_error(int line){
    cerr << "Illegal section specifier at line " << line << '\n';
}

void parse::report_parameter_error(int line){
    cerr << "Illegal parameter passed at line " << line << '\n';
}
