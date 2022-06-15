#include "defs.h"


int main(int argc, char *argv[1024]) {
    int i;
    char cmd[1024];
    
    for (i = 7; i < 79; i++) {
        sprintf(cmd, "./mem_one cg_standard_%i 0.01 0.005", i);
        system(cmd);
        sprintf(cmd, "./mem_one cg_sqrt_%i 0.01 0.005", i);
        system(cmd);
        sprintf(cmd, "./mem_one cg_pow2_%i 0.01 0.005", i);
        system(cmd);
        sprintf(cmd, "./mem_one cg_pow3_%i 0.01 0.005", i);
        system(cmd);
        sprintf(cmd, "./mem_one cg_sep_%i 0.01 0.005", i);
        system(cmd);
    }
    
    

    return 0;
}