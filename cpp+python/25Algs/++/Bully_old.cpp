#include "Bully.h"

Bully::Bully() {
	printf("In incomplete Bully constructor\n");
	exit(1);
}

Bully::Bully(int _me, int _A[NUM_PLAYERS], double ***_M) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;

	t = 0;
	deriveStrategies();	
}

void Bully::deriveStrategies() {
	int i, j, k, val1, val2, num, number;
	double *minner, high = -9999;;
	
	numStates = A[0] * A[1];
	numStrats = pow(A[me], numStates);

	int *strats = new int[numStates];
	minner = new double[numStrats];
	theIStrats = new omegaPair*[numStrats];
	
	number = 0;
	
	for (i = 0; i < numStrats; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(A[1-me], j);
			num = i % val1;
			num = i - num;
			val2 = pow(A[1-me], j+1);
			num = num % val2;
			strats[j] = num / val1;
		}
		
		theIStrats[i] = new omegaPair(me, A, M, 1, FOLLOWER, strats);
		
		minner[i] = 1.0;
		printf("%i Valors (", i);
		theIStrats[i]->ares->printStrat();
		printf("):"); 
		for (j = 0; j < numStates; j++) {
			printf("%.3lf ", theIStrats[i]->ares->Valor[j]);
			if (theIStrats[i]->ares->Valor[j] < minner[i])
				minner[i] = theIStrats[i]->ares->Valor[j];
		}
		
		if (minner[i] > high) {
			a = i;
			high = minner[i];
			number = 1;
		}
		else if (minner[i] == high) {
			number ++;
		}
		
		printf("\n");
	}
	
	if (number > 1) {
		num = rand() % number;
		int c = 0;
		for (i = 0; i < numStrats; i++) {
			if (minner[i] == high)
				c ++;
			
			if (c > num) {
				a = i;
				break;
			}
		}
	}
	
	printf("picked %i at %.3lf\n", a, high);

	// find the steady state
	int acts[2];
	bool *visited = new bool[numStates];
	for (i = 0; i < numStates; i++)
		visited[i] = false;
	estado = rand() % numStates;
	while (!visited[estado]) {
		visited[estado] = true;
		acts[me] = theIStrats[a]->ares->selectAction(estado);
		acts[1-me] = theIStrats[a]->thars->selectAction(estado);
		estado = theIStrats[0]->ares->encodeJointAction(acts);
	}
	printf("starting in estado = %i\n", estado);
	
	delete strats;
	delete minner;
	delete visited;
}

Bully::~Bully() {
	int i;
	for (i = 0; i < numStrats; i++) {
		delete theIStrats[i];
	}
	delete theIStrats;
}

int Bully::move() {
	int act = theIStrats[a]->ares->selectAction(estado);
	//printf("%i -> %i\n", estado, act);
	return act;
}

void Bully::update(int *acts) {
	estado = theIStrats[0]->ares->encodeJointAction(acts);	
	t++;
}
