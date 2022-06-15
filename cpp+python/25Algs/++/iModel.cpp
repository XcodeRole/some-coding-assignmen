#include "iModel.h"

iModel::iModel() {
	omega = 1;
	me = 0;
	A[0] = A[1] = 2;
	
	init();
}

iModel::iModel(int _me, int _A[NUM_PLAYERS], int _omega) {
	int i;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	omega = _omega;
	
	init();
}

void iModel::init() {
	int i;
	
	int numJointActions = A[0];
	for (i = 1; i < NUM_PLAYERS; i++)
		numJointActions *= A[i];
	
	numStates = pow(numJointActions, omega);
	lastSeen = new int[numStates];
	lastTime = new int[numStates];
	
	for (i = 0; i < numStates; i++)
		lastSeen[i] = lastTime[i] = -1;
}

iModel::~iModel() {
	delete lastSeen;
	delete lastTime;
}

void iModel::update(int *acts, int estado, int _t) {
	int i;
	
	if ((acts[1-me] != lastSeen[estado]) && (lastSeen[estado] != -1)) {  // has the opponent changed its strategy?
		//printf("c");
		for (i = 0; i < numStates; i++)
			lastSeen[i] = -1;
		lastSeen[estado] = acts[1-me];
		lastTime[estado] = _t;
	}
	else {
		//printf("u");
		lastSeen[estado] = acts[1-me];
		lastTime[estado] = _t;
	}
	
	//print();
}

bool iModel::match(omegaStrategy *thars) {
	int s;
	
	for (s = 0; s < thars->numStates; s++) {
		if (lastSeen[s] >= 0) {
			if (thars->pi[s][lastSeen[s]] < 0.9999)
				return false;
		}
	}
	
	return true;
}

bool iModel::match(double **pi) {
	int s;
	
	for (s = 0; s < numStates; s++) {
		if (lastSeen[s] >= 0) {
			if (pi[s][lastSeen[s]] < 0.01)
				return false;
		}
	}
	
	return true;
}


void iModel::print() {
	int i;
	printf("iModel%i: ", me);
	for (i = 0; i < numStates; i++) {
		if (lastSeen[i] == -1)
			printf("?");
		else
			printf("%i", lastSeen[i]);//, lastTime[i]);
	}
	//printf("\n");
}
