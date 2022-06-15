#include "gene.h"


Gene::Gene() {
    printf("incomplete gene constructor\n");
    exit(1);
}

Gene::Gene(int *_A, double ***_M, double _genome[10]) {
    A = _A;
    M = _M;
    
    int i;
    for (i = 0; i < 10; i++) {
        genome[i] = _genome[i];
    }
    
    round = 0;
    score = 0.0;
    
    for (i = 0; i < 8; i++) {
        nuevo[i][0] = nuevo[i][1] = true;
    }
}

Gene::~Gene() {
}

void Gene::reset(double _genome[10], double mutationRate) {
    //printf("a"); fflush(stdout);
    int i;
    double num;
    for (i = 0; i < 10; i++) {
        num = rand() / (double)RAND_MAX;
        if (num < mutationRate)
            genome[i] = rand() / (double)RAND_MAX;
        else
            genome[i] = _genome[i];
    }
    
    //printf("b"); fflush(stdout);
    
    round = 0;
    score = 0.0;
    
    for (i = 0; i < 8; i++) {
        nuevo[i][0] = nuevo[i][1] = true;
    }
    
    //printf("c"); fflush(stdout);
}

int Gene::act(bool isRow, int index) {
    int s;
    
    if (nuevo[index][(int)isRow]) {
        if (isRow)
            s = 4;
        else
            s = 9;
    }
    else {
        if (isRow)
            s = rowPrevious[index][0] * A[1] + rowPrevious[index][1];
        else
            s = 5 + (colPrevious[index][0] * A[1] + colPrevious[index][1]);
    }

    int a = generateAction(genome[s]);

    return a;
}

double Gene::generateAction(double val) {
    double num = rand() / (double)RAND_MAX;

    if (num < val)
        return 0;
    else
        return 1;
}

void Gene::update(bool isRow, int *acts, int index) {
    if (isRow) {
        rowPrevious[index][0] = acts[0];
        rowPrevious[index][1] = acts[1];
        score += M[0][acts[0]][acts[1]];
        //printf("Row: %lf\n", M[0][acts[0]][acts[1]]);
    }
    else {
        colPrevious[index][0] = acts[0];
        colPrevious[index][1] = acts[1];
        score += M[1][acts[0]][acts[1]];
        //printf("Col: %lf\n", M[1][acts[0]][acts[1]]);
    }
    
    round ++;
    
    nuevo[index][(int)isRow] = false;
    
    oldScore = score;
    oldRound = round;
    for (int i = 0; i < 10; i++) {
        oldGenome[i] = genome[i];
    }
}