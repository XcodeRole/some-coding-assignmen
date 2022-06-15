#include "QLearner.h"


QLearner::QLearner() {
}

QLearner::QLearner(int _me, double ***_M, int _A[2], double _gamma, bool _setHigh) {
	int i, j;
	
    M = _M;
    A[0] = _A[0];
    A[1] = _A[1];
	
    me = _me;
	gamma = _gamma;

	numStates = A[0] * A[1];

	Q = new double*[numStates];
	cuenta = new int*[numStates];
	for (i = 0; i < numStates; i++) {
		Q[i] = new double[A[me]];
		cuenta[i] = new int[A[me]];
		for (j = 0; j < A[me]; j++) {
			Q[i][j] = ((double)rand() / (double)RAND_MAX) / (1.0 - gamma);
			cuenta[i][j] = 0;
		}
	}

	estado = -1;
	t = 0;

	r_max = -99999;
	setHigh = _setHigh;
	numRandomMoves = 10 + rand() % 11;	
}

QLearner::~QLearner() {
	int i;

	for (i = 0; i < numStates; i++) {
		delete Q[i];
		delete cuenta[i];
	}

	delete Q;
	delete cuenta;
	
	//fclose(fp);
}

int QLearner::move() {
	double exploration = 1.0 / (10.0 + (t / 10.0));
	int a;
	if ((((double)rand() / (double)RAND_MAX) < exploration) || (estado < 0) || (setHigh && (t < numRandomMoves)))
		a = rand() % A[me];
	else
		a = argmaxQ();
	
	return a;
}

void QLearner::update(int acts[2]) {
    double r_t = M[me][acts[0]][acts[1]];
    
	int sprime = acts[1] * A[0] + acts[0];
	if (r_t > r_max)
		r_max = r_t;
	
	if (setHigh && (t == numRandomMoves)) {
		int i, j;
		
		for (i = 0; i < numStates; i++) {
			for (j = 0; j < A[me]; j++) {
				Q[i][j] = r_max / (1.0 - gamma);
				cuenta[i][j] = 0;
			}
		}
	}
	
	if ((estado >= 0) && (sprime >= 0)) {
		double Alpha = 1.0 / (10.0 + ((double)(cuenta[estado][acts[me]]) / 100.0));
		Q[estado][acts[me]] = (1.0 - Alpha) * Q[estado][acts[me]] + 
									Alpha * (r_t + gamma * maxQ(sprime));

		cuenta[estado][acts[me]] ++;
	}
	estado = sprime;
	t ++;
}

int QLearner::argmaxQ() {
	int i;
	double max = Q[estado][0];
	int index = 0;

	for (i = 1; i < A[me]; i++) {
		if ((Q[estado][i] > max) || ((Q[estado][i] == max) && (rand() % 2))) {
			index = i;
			max = Q[estado][i];
		}
	}

	return index;
}

double QLearner::maxQ(int s) {
	int i;
	double max = Q[s][0];
	int index = 0;

	for (i = 1; i < A[me]; i++) {
		if (Q[s][i] > max) {
			index = i;
			max = Q[s][i];
		}
	}

	return max;
}
