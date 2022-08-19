#include "vm.h"
#include "main_util.h"
#include <stdio.h>

int main(int argc, char** argv){
    if (argc != 2){
        printf("Syntax: vm SOURCE\n");
        return -1;
    } else {
        if (read_source(argv[1]) != -1){
            run_machine();
        } else {
            printf("Sorry, an error occured\n");
            return -1;
        }
    }
    return 0;
}
