#include "Memory1.h"


Memory1::Memory1() {
    printf("empty Memory1 constructor\n");
    exit(1);
}

Memory1::Memory1(int *_A, int _me, char _game[1024]) {
    A = _A;
	me = _me;

    printf("Memory1 constructor\n"); fflush(stdout);

    char fname[1024];
    sprintf(fname, "../Memory_one/%s.txt", _game);
    FILE *fp = fopen(fname, "r");
    
    if (fp == NULL) {
        printf("The StochasticMemory strategy for the game %s has not been created\n", fname); fflush(stdout);
        printf("Creating it now (this will only work if you've compiled the code in the folder StochasticMemory) ... \n");
        printf("Please be patient, it is going to take 5-10 minutes or so\n");

        char sysLlamado[1024];
        sprintf(sysLlamado, "../StochasticMemory/mem_one %s 0.01 0.005", _game);
        system(sysLlamado);
        
        fp = fopen(fname, "r");
    }
    
    int i;
    for (i = 0; i < 10; i++)
        fscanf(fp, "%lf", &(genoType[i]));
    fclose(fp);
    
    nuevo = true;
}

Memory1::~Memory1() {
}
	
int Memory1::move() {
    int s;
    
    if (nuevo) {
        if (me == 0)
            s = 4;
        else
            s = 9;
    }
    else {
        if (me == 0)
            s = previous[0] * A[1] + previous[1];
        else
            s = 5 + (previous[0] * A[1] + previous[1]);
    }

    int a = generateAction(genoType[s]);

    return a;
}

double Memory1::generateAction(double val) {
    double num = rand() / (double)RAND_MAX;

    if (num < val)
        return 0;
    else
        return 1;
}

void Memory1::update(int *acts) {
    previous[0] = acts[0];
    previous[1] = acts[1];
    
    nuevo = false;
}
	
