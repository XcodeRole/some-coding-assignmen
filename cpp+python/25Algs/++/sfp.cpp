#include "sfp.h"

sFP::sFP() {
}

sFP::sFP(int _nActions, int _priorK, int _me, int _omega, bool _verbose, double ***_M) {
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
	
    printf("numStates = %i\n", numStates);
    
	utilities = new double[nActions];
	pi = new double[nActions];
	
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
	
/*	verbose = _verbose;
	if (verbose) {
		char nombre[1024];
		sprintf(nombre, "sfp%i.dat\0", me);
		fp = fopen(nombre, "w");
	}
	else
		fp = NULL;
	
	mnmx = new minimaxLog(_nActions);	*/
}

sFP::~sFP() {
	int i;
	//for (i = 0; i < nActions; i++) {
	//	delete payoffs[i];
	//}
	//delete payoffs;
	delete utilities;
	delete pi;
	for (i = 0; i < numStates; i++)
		delete K[i];
	delete K;
	
	//if (verbose)
	//	fclose(fp);
}

int sFP::move() {
    //printf("move\n"); fflush(stdout);
	//if ((rand() % 1000) < (exploration * 1000.0))
	//	return rand() % nActions;
	//else {
	//	int a = bestResponse();
	//	return a;
	//}
	
	return bestResponse();
}

void sFP::update(int *acts) {
    //printf("update\n"); fflush(stdout);

	if (estado != -1) {
		K[estado][acts[1-me]] ++;
	}
	t ++;
	
	estado = 0;//getNewState(acts);
    
    //printf("estado = %i\n", estado);
    
	//updateStateHistory(estado, acts);
}

int sFP::bestResponse() {
	if (estado == -1)
		return rand() % nActions;
	
    //printf("estado = %i\n", estado); fflush(stdout);
    
	int i, j;
	int totalcount = 0;
	for (j = 0; j < nActions; j++) {
		totalcount += K[estado][j];
	}
	
    //printf("totalcount = %i\n", totalcount); fflush(stdout);
    
	if (totalcount == 0)
		return rand() % nActions;
	
	double denom = 0;
	double tau = 1.0 / ((t+1) / 100.0);
	if (tau < 0.01)
		tau = 0.01;
	for (i = 0; i < nActions; i++) {
		utilities[i] = 0.0;
		for (j = 0; j < nActions; j++) {
            if (me == 0)
                utilities[i] += M[me][i][j] * (K[estado][j] / (double)totalcount);
            else
                utilities[i] += M[me][j][i] * (K[estado][j] / (double)totalcount);
			//utilities[i] +=  scaled(payoffs[i][j]) * (K[estado][j] / (double)totalcount);
		}
		denom += exp((1.0 / tau) * utilities[i]);
	}
	
	for (i = 0; i < nActions; i++) {
		pi[i] = exp((1.0 / tau) * utilities[i]) / denom;
	}
	
	//if (verbose)
	//	dump();
	
	return generateAction(pi);
}

int sFP::generateAction(double *x) {
	int num = rand();
	int i;
	
	double sum = 0.0;
	for (i = 0; i < nActions; i++) {
		sum += x[i] * RAND_MAX;
		if (num <= sum) {
			return i;
		}
	}
	
	// error statements
	printf("i (me = %i) never selected an action: num = %i\n", me, num);
	for (i = 0; i < nActions; i++) {
		printf("%lf ", x[i]);
	}
	printf("\n");
	exit(1);
	
	return -1;
}

double sFP::scaled(double val) {
	return (val - lowval) / (highval - lowval);
}

void sFP::updateStateHistory(int ns, int *actions) {
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

int sFP::getNewState(int *actions) {
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
void sFP::loadPayoffMatrix(double **pm, int _numJointActions, int _me) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < nActions; i++) {
		for (j = 0; j < nActions; j++) {
			if (_me == 1) {
				payoffs[i][j] = pm[count][_me];
				if (highval < payoffs[i][j])
					highval = payoffs[i][j];
				if (lowval > payoffs[i][j])
					lowval = payoffs[i][j];	
			}
			else {
				payoffs[j][i] = pm[count][_me];
				if (highval < payoffs[j][i])
					highval = payoffs[j][i];
				if (lowval > payoffs[j][i])
					lowval = payoffs[j][i];	
			}
			count ++;
		}
	}
}

void sFP::loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < nActions; i++) {
		for (j = 0; j < nActions; j++) {
			if (_me == 1) {
				payoffs[i][j] = pm[count][_me];
				if (highval < payoffs[i][j])
					highval = payoffs[i][j];
				if (lowval > payoffs[i][j])
					lowval = payoffs[i][j];	
			}
			else {
				payoffs[j][i] = pm[count][_me];
				if (highval < payoffs[j][i])
					highval = payoffs[j][i];
				if (lowval > payoffs[j][i])
					lowval = payoffs[j][i];	
			}
			count ++;
		}
	}
	
	// assume square matrix
	mnmx->mv = _mv;
	int dim = (int)sqrt(_numJointActions);
	for (i = 0; i < dim; i++)
		mnmx->ms[i] = _ms[i];
}


void sFP::dump() {
	int i;
	
	fprintf(fp, "%i:\t", t);
	for (i = 0; i < nActions; i++) {
		fprintf(fp, "%.3lf (%.2lf)\t", pi[i], utilities[i]);
	}
	fprintf(fp, "\n");
}
*/
