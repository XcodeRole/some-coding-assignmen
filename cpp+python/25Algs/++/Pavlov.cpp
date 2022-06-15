#include "Pavlov.h"


Pavlov::Pavlov() {
    printf("empty Pavlov constructor\n");
    exit(1);
}

Pavlov::Pavlov(int *_A, int _me, double ***_M) {
    M = _M;
    A = _A;
	me = _me;
    
    threshold = 0.0;
    int i, j;
    for (i = 0; i < A[0]; i++) {
        for (j = 0; j < A[1]; j++) {
            threshold += M[me][i][j];
        }
    }
    threshold /= A[0] * A[1];
    
    printf("threshold = %lf\n", threshold);
    
    curAction = rand() % 2;
}

Pavlov::~Pavlov() {
}
	
int Pavlov::move() {
    return curAction;
}

void Pavlov::update(int *acts) {
    if (M[me][acts[0]][acts[1]] < threshold)
        curAction = 1-curAction;
}
	
