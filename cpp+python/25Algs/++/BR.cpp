#include "BR.h"

BR::BR() {
	printf("entered into incomplete BR constructor\n");
}

BR::BR(int _me, int _A[NUM_PLAYERS], double ***_M) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	
	verbose = false;
	
	myBR = new omegaPair(me, A, M, 1, UNKNOWN);
	
	kappa = new int*[myBR->thars->numStates];
	for (i = 0; i < myBR->thars->numStates; i++) {
		kappa[i] = new int[A[1-me]];
		for (j = 0; j < A[1-me]; j++)
			kappa[i][j] = 0;
	}
	
	estado = -1;
	t = 0;
}

BR::~BR() {
	for (int i = 0; i < myBR->thars->numStates; i++)
		delete kappa[i];
	
	delete myBR;
	delete kappa;
}

int BR::move() {
	double num = (double)(rand()) / RAND_MAX;
	if ((num < 0.1) || (estado < 0))
		return rand() % A[me];
	
	return myBR->ares->selectAction(estado);
}

void BR::update(int *acts) {
	int sprime = myBR->ares->encodeJointAction(acts);
	if (estado >= 0) {
		kappa[estado][acts[1-me]] ++;
		myBR->updateUnknown(kappa, estado);
	}
	
	//if (((t % 100) == 0) && (t > 0))
	//	print();
	
	estado = sprime;
	t++;
}

void BR::print() {
	int i, j;
	
	printf("\n\n%i: Distribution of opponent actions:\n", me);
	for (i = 0; i < myBR->ares->numStates; i++) {
		printf("%i: ", i);
		for (j = 0; j < A[1-me]; j++) {
			printf("%i ", kappa[i][j]);
		}
		printf("\n");
	}
	
	printf("\n%i: Values:\n", me);
	for (i = 0; i < myBR->ares->numStates; i++) {
		printf("%i: ", i);
		for (j = 0; j < A[1-me]; j++) {
			printf("%.3lf ", myBR->ares->V[i][j]);
		}
		printf("\n");
	}
}
