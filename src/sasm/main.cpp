#include <iostream>
#include <cstring>
#include "assembler.hpp"

int main(int argc, char **argv){
    if ((argc != 2) && (argc != 3)) {
        std::cout << "Syntax: sasm SOURCE [SINK] \n";
        return -1;
    } else {
        char sink_file[256];
        if (argc == 2)
            std::strcpy(sink_file, "a.sam");
        else
            std::strcpy(sink_file, argv[2]);
        assemble::SamAssembler assembler(argv[1], sink_file);
        return assembler.assemble();
    }
}
