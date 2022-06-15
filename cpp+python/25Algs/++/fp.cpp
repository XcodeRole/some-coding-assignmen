#include "fp.h"

FP::FP() {
}

FP::FP(int _nActions, int _priorK, int _me, int _omega, double ***_M) {
	nActions = _nActions;
    M = _M;
	
	// Assumptions: (1) 2-players; (2) all players have same number of actions
	int i, j;
/*	payoffs = new double*[nActions];
	for (i = 0; i < nActions; i++) {
		payoffs[i] = new double[nActions];
		for (j = 0; j < nActions; j++) {
			payoffs[i][j] = 0;
		}
	}*/
	omega = _omega;
	numStates = nActions;
	for (i = 1; i < 2; i++) // assume nAgents = 2
		numStates *= nActions;
	numJointActions = numStates;
	numStates = (int)pow((double)numStates, (double)omega);

	K = new int*[numStates];
	for (i = 0; i < numStates; i++) {
        K[i] = new int[nActions];
		for (j = 0; j < nActions; j++) {
			K[i][j] = _priorK;
		}
	}

	me = _me;
	estado = 0;
	t = 0;

	stateHistory = new int[numStates];
	for (i = 0; i < numStates; i++)
		stateHistory[i] = -1;
	
	jointActionHistory = new int[omega];
	for (i = 0; i < omega; i++)
		jointActionHistory[i] = -1;
	
	//mnmx = new minimaxLog(nActions);
}

FP::~FP() {
	int i;

	//for (i = 0; i < nActions; i++) {
	//	delete payoffs[i];
	//}
	//delete payoffs;

	for (i = 0; i < numStates; i++) //nActions * nActions; i++)
		delete K[i];
	delete K;
}

int FP::move() {
	return bestResponse();
}

void FP::update(int *acts) {
    Update(acts, M[me][acts[0]][acts[1]]);

}

void FP::Update(int *actions, double r_t) {
	if (estado != -1) {
		K[estado][actions[1-me]] ++;
	}
	t ++;

	//estado = getNewState(actions);
	//updateStateHistory(estado, actions);
}

int FP::bestResponse() {
	//if (estado == -1)
	//	return rand() % nActions;

	int i, j;
	int totalcount = 0;
	for (j = 0; j < nActions; j++) {
		totalcount += K[estado][j];
	}

	if (totalcount == 0)
		return rand() % nActions;

	double value, sum, max = -99999;
	int maxIndex = -1;
	for (i = 0; i < nActions; i++) {
		sum = 0.0;
		for (j = 0; j < nActions; j++) {
            if (me == 0)
                sum +=  M[me][i][j] * (K[estado][j] / (double)totalcount);
            else
                sum +=  M[me][j][i] * (K[estado][j] / (double)totalcount);
		}
		if ((sum > max) || ((sum == max) && (rand() % 2))) {
			max = sum;
			maxIndex = i;
		}
	}

	return maxIndex;
}

void FP::updateStateHistory(int ns, int *actions) {
	int i;

	for (i = 0; i < numStates - 1; i++) 
		stateHistory[i] = stateHistory[i+1];

	stateHistory[numStates-1] = ns;

	int sprime = 0;
	for (i = 0; i < 2; i++) { // assume nAgents = 2
		sprime += (int)(pow(nActions, i)) * actions[i];
	}
	for (i = 0; i < omega - 1; i++) {
		jointActionHistory[i] = jointActionHistory[i+1];
	}
	jointActionHistory[omega-1] = sprime;
}

int FP::getNewState(int *actions) {
	int i, newState = 0;
	int tier = 1;

	for (i = 1; i < omega; i++) {
		if (jointActionHistory[i] == -1)
			return -1;

		newState += tier * jointActionHistory[i];
		tier *= numJointActions;
	}

	int sprime = 0;
	for (i = 0; i < 2; i++) { // assume nAgents = 2
		sprime += (int)(pow(nActions, i)) * actions[i];
	}

	newState += sprime * tier;

	return newState;
}
/*
void FP::loadPayoffMatrix(double **pm, int _numJointActions, int _me) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < nActions; i++) {
		for (j = 0; j < nActions; j++) {
			if (_me == 1) {
				payoffs[i][j] = pm[count][_me];
			}
			else {
				payoffs[j][i] = pm[count][_me];
			}
			count ++;
		}
	}
	
	pmatrix = new double[_numJointActions];
	for (i = 0; i < _numJointActions; i++) {
		pmatrix[i] = pm[i][_me];
	}
	
	// assume square matrix
	int dim = (int)sqrt(_numJointActions);
	int nActs[2];
	nActs[0] = dim;
	nActs[1] = dim;
	
	mnmx->getMinimax(nActs, _me, pmatrix);	
	printf("mv = %lf\n", mnmx->mv);
}

void FP::loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < nActions; i++) {
		for (j = 0; j < nActions; j++) {
			if (_me == 1) {
				payoffs[i][j] = pm[count][_me];
			}
			else {
				payoffs[j][i] = pm[count][_me];
			}
			count ++;
		}
	}
	
	pmatrix = new double[_numJointActions];
	for (i = 0; i < _numJointActions; i++) {
		pmatrix[i] = pm[i][_me];
	}
	
	// assume square matrix
	mnmx->mv = _mv;
	int dim = (int)sqrt(_numJointActions);
	for (i = 0; i < dim; i++)
		mnmx->ms[i] = _ms[i];
}
*/
