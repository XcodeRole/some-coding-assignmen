#include "EEE.h"

EEE::EEE() {
	printf("In incomplete EEE constructor\n");
	exit(1);
}

EEE::EEE(int _me, int _A[NUM_PLAYERS], double ***_M) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	
	deriveStrategies();
	
	M_e = new double[numStrats];
	N_e = new int[numStrats];
	S_e = new int[numStrats];
	for (i = 0; i < numStrats; i++) {
		N_e[i] = 0;
		M_e[i] = 0.0;
		S_e[i] = 0.0;
	}
	
	a = -1;
	t = 0;
	n_i = 4;
	estado = -1;
	count = 0;
}

void EEE::deriveStrategies() {
	int i, j, k, val1, val2, num;
	
	numStates = A[0] * A[1];
	int numCombos = pow(A[me], numStates);
	numStrats = numCombos + 1;
	//printf("numStrats = %i\n", numStrats);
	
	theStrats = new omegaStrategy*[numStrats];
	int *strats = new int[numStates];
	
	for (i = 0; i < numCombos; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(A[me], j);
			num = i % val1;
			num = i - num;
			val2 = pow(A[me], j+1);
			num = num % val2;
			strats[j] = num / val1;
		}
		
		theStrats[i] = new omegaStrategy(me, A, 1);
		for (j = 0; j < numStates; j++) {
			for (k = 0; k < A[me]; k++)
				theStrats[i]->pi[j][k] = 0.0;
			theStrats[i]->pi[j][strats[j]] = 1.0;
		}
	}
	
	minimaxStuff();
	
	delete strats;
}

void EEE::minimaxStuff() {
	int i, j;
	
	minimaxLog *mnmx = new minimaxLog(A[me]);
	double *payoff = new double[numStates];
	int c = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[c] = M[me][i][j];
			c ++;
		}
	}
	mnmx->getMinimax(A, me, payoff);
	delete payoff;
	if (!isPure(mnmx->ms)) {
		theStrats[numStrats-1] = new omegaStrategy(me, A, 1);
		for (j = 0; j < numStates; j++) {
			for (i = 0; i < A[me]; i++)
				theStrats[numStrats-1]->pi[j][i] = mnmx->ms[i];
		}
	}
	else
		numStrats --;
	delete mnmx;
}

EEE::~EEE() {
	delete M_e;
	delete N_e;
	delete S_e;
	
	int i;
	for (i = 0; i < numStrats; i++)
		delete theStrats[i];
	delete theStrats;
}

int EEE::move() {
	if (t == 0)
		return rand() % A[me];
	
	if (count == 0) {
		double prob = 0.1;
		double num = ((double)rand()) / RAND_MAX;
		if (num < prob) {
			//printf("R");
			a = rand() % numStrats;
		}
		else {
			double best = -99999;
			a = -1;
			int bestCount = 0;
			int i;
			for (i = 0; i < numStrats; i++) {
				if (M_e[i] > best) {
					best = M_e[i];
					a = i;
					bestCount = 1;
				}
				else if (M_e[i] == best) {
					bestCount ++;
				}
			}
			
			if (bestCount > 1) {
				int num2 = rand() % bestCount;
				int c = 0;
				for (i = 0; i < numStrats; i++) {
					if (M_e[i] == best) {
						//printf("%i: %i is bestCount of %i (%i)\n", me, i, bestCount, num2);
						c ++;
					}
					
					if (c > num2) {
						a = i;
						//printf("%i: broke tie with %i\n", me, i);
						break;
					}
				}
			}
		}
		
	}
	
	return theStrats[a]->selectAction(estado);
}

void EEE::update(int *acts) {
	estado = theStrats[0]->encodeJointAction(acts);
	
	if (t > 0) {
		r += M[me][acts[0]][acts[1]];
		count ++;
		
		if (count == n_i) {
			N_e[a] ++;
			S_e[a] += n_i;
			double R = r / count;
			M_e[a] += ((double)n_i / S_e[a]) * (R - M_e[a]);
			
			count = 0;
			r = 0.0;
		}
	}
	
	t++;
}

bool EEE::isPure(double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if ((v[i] > 0.0) && (v[i] < 1.0))
			return false;
	}
	return true;
}

