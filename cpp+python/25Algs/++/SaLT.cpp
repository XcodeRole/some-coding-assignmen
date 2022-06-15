#include "SaLT.h"

SaLT::SaLT() {
	printf("In incomplete SaLT constructor\n");
	exit(1);
}

SaLT::SaLT(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	lambda = _lambda;

	aspiration = -99999;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			if (M[me][i][j] > aspiration)
				aspiration = M[me][i][j];
		}
	}
	
	r = -99999;
	a = -1;
}

SaLT::~SaLT() {
}

int SaLT::move() {
	//printf("aspiration = %.2lf; r = %.2lf\n", aspiration, r);
	if (aspiration > r)
		a = rand() % A[me];
	
	return a;
}

void SaLT::update(int *acts) {
	r = M[me][acts[0]][acts[1]];
	aspiration = lambda * aspiration + (1.0 - lambda) * r;
}
