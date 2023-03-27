#include "assembler.hpp"

int main(int argc, char **argv) {
    assemble::SamAssembler assembler(argv[1], argv[2]);
    assembler.assemble();
    return 0;
}
