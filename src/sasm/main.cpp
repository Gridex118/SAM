#include <iostream>
#include "assembler.hpp"

int main(int argc, char **argv){
    if ((argc != 2) && (argc != 3)) {
        std::cout << "Syntax: sasm SOURCE [SINK] \n";
    } else {
        char *sink_file;
        if (argc == 2)
            sink_file = "a.sam";
        else
            sink_file = argv[2];
        assemble::SamAssembler assembler(argv[1], sink_file);
        if (assembler.assemble() == -1) {
            return -1;
        }
    }
    return 0;
}
