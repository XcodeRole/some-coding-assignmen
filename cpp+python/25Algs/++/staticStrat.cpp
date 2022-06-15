#include "staticStrat.h"

staticStrat::staticStrat() {
	printf("incomplete staticStrat constructor\n");
	exit(1);
}

staticStrat::staticStrat(int _me, int _A[NUM_PLAYERS], double ***_M, char *strStrat) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	
	numStates = A[0] * A[1];
	estado = -1;
	
	if (strStrat[0] == 'm') {
		minmax = true;
		mnmx = new minimaxLog(A[me]);
		double *payoff = new double[numStates];
		int c = 0;
		for (i = 0; i < A[0]; i++) {
			for (j = 0; j < A[1]; j++) {
				payoff[c] = M[me][i][j];
				c ++;
			}
		}
		mnmx->getMinimax(A, me, payoff);
	}
	else {
		minmax = false;
		actions = new int[numStates];
		
		for (i = 0; i < numStates; i++) {
			if (strStrat[i] == '0')
				actions[i] = 0;
			else
				actions[i] = 1;
		}
	}
}

staticStrat::~staticStrat() {
	if (!minmax)
		delete actions;
	else
		delete mnmx;
}

int staticStrat::move() {
	if (minmax) {
		double sum = 0.0;
		double num = rand() / (double)RAND_MAX;
		for (int i = 0; i < A[me]; i++) {
			sum += mnmx->ms[i];
			if (sum > num)
				return i;
		}
		
		return A[me] - 1;
	}
	
	if (estado < 0)
		return rand() % A[me];
	
	return actions[estado];
}

void staticStrat::update(int *acts) {
	estado = A[1] * acts[0] + acts[1];;
}
