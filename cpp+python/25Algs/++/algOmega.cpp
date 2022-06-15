#include "algOmega.h"

algOmega::algOmega() {
	printf("incomplete algOmega constructor\n");
	exit(1);
}

algOmega::algOmega(int _me, int _A[NUM_PLAYERS], double ***_M, double _decay) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	decay = _decay;
	
	im = new iModel(me, A, 1);
	br = new BR(me, A, M);
	
	imBR = new omegaPair(me, A, M, 1, UNKNOWN);
	currentImKappa = new int*[imBR->thars->numStates];
	for (i = 0; i < imBR->thars->numStates; i++) {
		currentImKappa[i] = new int[A[1-me]];
		for (j = 0; j < A[1-me]; j++)
			currentImKappa[i][j] = 1;
	}
}

algOmega::~algOmega() {
	for (int i = 0; i < imBR->thars->numStates; i++)
		delete currentImKappa[i];
	delete im;
	delete br;
}

int algOmega::move() {
	theta = 1.0 - decay * br->t;
	if (theta < 0)
		theta = 0.0;
	
	double num = (double)(rand()) / RAND_MAX;
	if (num < theta) {
		num = (double)(rand()) / RAND_MAX;
		if ((num < 0.1) || (br->estado < 0)) {
			printf("r-");
			return rand() % A[me];
		}
		
		printf("i-");
		return imBR->ares->selectAction(br->estado);
	}
	else {
		printf("a-");
		return br->myBR->ares->selectAction(br->estado);
	}
}

void algOmega::update(int *acts) {
	if (br->estado >= 0)
		im->update(acts, br->estado, br->t);
	br->update(acts);

	updateImBR();
/*	
	if ((br->t % 100) == 0) {
		int j;
		printf("\ncurrentImKappa (for %i): ", me);
		for (j = 0; j < imBR->thars->numStates; j++)
			printf("%.2lf ", imBR->thars->pi[j][0]);
		printf("\n   BR: ");
		for (j = 0; j < imBR->ares->numStates; j++)
			printf("%i   ", imBR->ares->argmax(j));
		printf("\n V_me: ");
		for (j = 0; j < imBR->ares->numStates; j++)
			printf("%.2lf ", imBR->ares->Valor[j]);
		printf("\n");
		//ares->printV();
	}*/
}

void algOmega::updateImBR() {
	int i, j, sum;
	for (i = 0; i < imBR->ares->numStates; i++) {
		if (im->lastSeen[i] >= 0) {
			for (j = 0; j < A[1-me]; j++)
				currentImKappa[i][j] = 0;
			
			currentImKappa[i][im->lastSeen[i]] = 1;
		}
		else {
			for (j = 0; j < A[1-me]; j++)
				currentImKappa[i][j] = br->kappa[i][j];
		}
		
		sum = 0;
		for (j = 0; j < A[1-me]; j++)
			sum += currentImKappa[i][j];
		for (j = 0; j < A[1-me]; j++)
			imBR->thars->pi[i][j] = (double)(currentImKappa[i][j]) / (double)sum;		
	}
	
	imBR->ares->bestResponse(M, imBR->thars);
//	imBR->thars->valuesGivenStrategyPair(M, ares);
/*	
	bool verbose = false;
	
	if (verbose && (me == 0)) {
		printf("kappa: ");
		for (j = 0; j < thars->numStates; j++)
			printf("%.2lf ", thars->pi[j][0]);
		printf("\n   BR: ");
		for (j = 0; j < ares->numStates; j++)
			printf("%i   ", ares->argmax(j));
		printf("\n V_me: ");
		for (j = 0; j < ares->numStates; j++)
			printf("%.2lf ", ares->Valor[j]);
		printf("\n");
		//ares->printV();
	}
*/	
}
