#include "UO.h"

UO::UO() {
	printf("In incomplete UO constructor\n");
	exit(1);
}

UO::UO(int _me, int _A[NUM_PLAYERS], double ***_M) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;

	deriveStrategies();
	
	x = new double[numStrats];
	n = new int[numStrats];
	
	for (i = 0; i < numStrats; i++) {
		n[i] = 0;
		x[i] = 0.0;
	}
	
	num = 0;
	count = 0;
	r = 0.0;
	a = -1;
	init = true;
	t = 0;
	estado = -1;
}

void UO::deriveStrategies() {
	int i, j, k, val1, val2, num;
	
	numStates = A[0] * A[1];
	int numCombos = pow(A[me], numStates);
	numStrats = numCombos + 1;
	//printf("numStrats = %i\n", numStrats);

	beenThere = new bool[numStates];
	
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
	
	//printf("theStrats[0]->A[me] = %i\n", theStrats[0]->A[me]);
	//fflush(stdout);
	
	minimaxStuff();
	
	delete strats;
}

void UO::minimaxStuff() {
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

UO::~UO() {
	delete x;
	delete n;
	delete beenThere;

	int i;
	
	for (i = 0; i < numStrats; i++)
		delete theStrats[i];
	delete theStrats;
}

int UO::move() {
	if (t == 0)
		return rand() % A[me];
	
	if (init && (count == 0)) {
		bool *left = new bool[numStrats];
		int i, numLeft = 0;
		for (i = 0; i < numStrats; i++) {
			if (n[i] == 0) {
				left[i] = true;
				numLeft ++;
			}
			else
				left[i] = false;
		}
		
		if (numLeft == 0) {
			init = false;
			delete left;
		}
		else {
			a = rand() % numStrats;
			while (!(left[a]))
				a = rand() % numStrats;
			delete left;

			//printf("exploring %i\n", a);
			
			int act = theStrats[a]->selectAction(estado);
			
			return act;
		}
	}
	
	if (count == 0) {
		a = 0;
		double best = (x[a] / n[a]) + sqrt(2.0 * log((double)num) / n[a]);
		//printf("0: %.3lf (%i; %.3lf)\n", best, n[0], x[0] / n[0]);
		double val;
		for (int i = 1; i < numStrats; i++) {
			val = (x[i] / n[i]) + sqrt(2.0 * log((double)num) / n[i]);
			//printf("%i: %.3lf (%i; %.3lf)\n", i, val, n[i], x[i] / n[i]);
			if ((val > best) || ((val == best) && (rand() % 2))) {
				a = i;
				best = val;
			}
		}
		
		//printf("chose %i\n", a);
	}
	
	//printf("playing %i\n", a);
	
	//if ((rand() % 100) == 0)
	//	return rand() % A[me];
	
	return theStrats[a]->selectAction(estado);
}

void UO::update(int *acts) {
	estado = theStrats[0]->encodeJointAction(acts);
	
	if (t > 0) {
		r += M[me][acts[0]][acts[1]];
		count ++;
		
		if (beenThere[estado]) {
			//printf("outcome = %.3lf\n", r / count);
			x[a] += r;
			n[a] += count;
			num += count;
			
			//printf("updated %i: %.3lf, %i, %i\n", a, x[a] / n[a], n[a], num);

			count = 0;
			r = 0.0;
			
			for (int i = 0; i < numStates; i++)
				beenThere[i] = false;
		}
	}
	
	beenThere[estado] = true;
	
	t++;
}

bool UO::isPure(double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if ((v[i] > 0.0) && (v[i] < 1.0))
			return false;
	}
	return true;
}

