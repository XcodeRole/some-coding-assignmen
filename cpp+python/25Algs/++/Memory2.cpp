#include "Memory2.h"


Memory2::Memory2() {
    printf("empty Memory2 constructor\n");
    exit(1);
}

Memory2::Memory2(int *_A, int _me, char _game[1024]) {
    A = _A;
	me = _me;

    char fname[1024];
    sprintf(fname, "../Memory_two/%s.txt", _game);
    FILE *fp = fopen(fname, "r");

    if (fp == NULL) {
        printf("The StochasticMemory strategy for the game %s has not been created\n", fname); fflush(stdout);
        printf("Creating it now (this will only work if you've compiled the code in the folder StochasticMemory) ... \n");
        printf("Please be patient, it is going to take 5-10 minutes\n");

        char sysLlamado[1024];
        sprintf(sysLlamado, "../StochasticMemory/mem_one %s 0.01 0.005", _game);
        system(sysLlamado);
        
        fp = fopen(fname, "r");
    }

    int i;
    for (i = 0; i < 34; i++)
        fscanf(fp, "%lf", &(genoType[i]));
    fclose(fp);
    
    previous[0] = previous[1] = previous2[0] = previous2[1] = -1;
}

Memory2::~Memory2() {
}
	
int Memory2::move() {
    int s;
    
    if (previous2[0] == -1) {
        if (me == 0)
            s = 16;
        else
            s = 33;
    }
    else {
        if (me == 0)
            s = (4 * (previous2[0] * A[1] + previous2[1])) + (previous[0] * A[1] + previous[1]);
        else
            s = (4 * (previous2[0] * A[1] + previous2[1])) + (17 + (previous[0] * A[1] + previous[1]));
    }

    int a = generateAction(genoType[s]);

    return a;
}

double Memory2::generateAction(double val) {
    double num = rand() / (double)RAND_MAX;

    if (num < val)
        return 0;
    else
        return 1;
}

void Memory2::update(int *acts) {
    previous2[0] = previous[0];
    previous2[1] = previous[1];

    previous[0] = acts[0];
    previous[1] = acts[1];
}
	
