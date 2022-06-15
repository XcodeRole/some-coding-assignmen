#include "Gene2.h"


Gene2::Gene2() {
    printf("incomplete Gene2 constructor\n");
    exit(1);
}

Gene2::Gene2(int *_A, double ***_M, double _genome[34]) {
    A = _A;
    M = _M;
    
    int i;
    for (i = 0; i < 34; i++) {
        genome[i] = _genome[i];
    }
    
    round = 0;
    score = 0.0;
    
    for (i = 0; i < 8; i++) {
        nuevo[i][0] = nuevo[i][1] = true;
        rowPrevious[i][0] = rowPrevious[i][1] = rowPrevious2[i][0] = rowPrevious2[i][1] = -1;
        colPrevious[i][0] = colPrevious[i][1] = colPrevious2[i][0] = colPrevious2[i][1] = -1;
    }
}

Gene2::~Gene2() {
}

void Gene2::reset(double _genome[34], double mutationRate) {
    int i;
    double num;
    for (i = 0; i < 34; i++) {
        num = rand() / (double)RAND_MAX;
        if (num < mutationRate)
            genome[i] = rand() / (double)RAND_MAX;
        else
            genome[i] = _genome[i];
    }
    
    round = 0;
    score = 0.0;
    
    for (i = 0; i < 8; i++) {
        nuevo[i][0] = nuevo[i][1] = true;
        rowPrevious[i][0] = rowPrevious[i][1] = rowPrevious2[i][0] = rowPrevious2[i][1] = -1;
        colPrevious[i][0] = colPrevious[i][1] = colPrevious2[i][0] = colPrevious2[i][1] = -1;
    }
}

void Gene2::restart(int neighbor, int index) {
    int s;
    
    nuevo[neighbor][index] = true;
    rowPrevious[neighbor][0] = rowPrevious[neighbor][1] = -1;
    rowPrevious2[neighbor][0] = rowPrevious2[neighbor][1] = -1;
    colPrevious[neighbor][0] = colPrevious[neighbor][1] = -1;
    colPrevious2[neighbor][0] = colPrevious2[neighbor][1] = -1;

}

int Gene2::act(bool isRow, int index) {
    int s;
    
    if (nuevo[index][(int)isRow]) {
        if (isRow)
            s = 16;
        else
            s = 33;
    }
    else {
        if (isRow)
            s = (4 * (rowPrevious2[index][0] * A[1] + rowPrevious2[index][1])) +
                (rowPrevious[index][0] * A[1] + rowPrevious[index][1]);
        else
            s = (4 * (colPrevious2[index][0] * A[1] + colPrevious2[index][1])) +
                17 + (colPrevious[index][0] * A[1] + colPrevious[index][1]);
    }

    int a = generateAction(genome[s]);

    return a;
}

double Gene2::generateAction(double val) {
    double num = rand() / (double)RAND_MAX;

    if (num < val)
        return 0;
    else
        return 1;
}

void Gene2::update(bool isRow, int *acts, int index) {
    if (isRow) {
        rowPrevious2[index][0] = rowPrevious[index][0];
        rowPrevious2[index][1] = rowPrevious[index][1];
        rowPrevious[index][0] = acts[0];
        rowPrevious[index][1] = acts[1];
        score += M[0][acts[0]][acts[1]];
        if (rowPrevious2[index][0] != -1)
            nuevo[index][(int)isRow] = false;
    }
    else {
        colPrevious2[index][0] = colPrevious[index][0];
        colPrevious2[index][1] = colPrevious[index][1];
        colPrevious[index][0] = acts[0];
        colPrevious[index][1] = acts[1];
        score += M[1][acts[0]][acts[1]];
        if (colPrevious2[index][0] != -1)
            nuevo[index][(int)isRow] = false;
    }
    
    round ++;
    
    
    oldScore = score;
    oldRound = round;
    for (int i = 0; i < 34; i++) {
        oldGenome[i] = genome[i];
    }
}