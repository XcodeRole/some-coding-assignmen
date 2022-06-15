#include "Rmax.h"

Rmax::Rmax() {
	printf("incomplete Rmax constructor\n");
	exit(1);
}

Rmax::Rmax(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _K, double _gamma) {
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	omega = _omega;
	K = _K;
	gamma = _gamma;

	numJointActions = A[0];
	for (i = 1; i < NUM_PLAYERS; i++)
		numJointActions *= A[i];	
	numStates = pow(numJointActions, omega);
	
	//printf("numStates = %i\n", numStates);
	//fflush(stdout);
	
	kappa = new int**[2];
	kappa[0] = new int*[numStates];
	kappa[1] = new int*[numStates];
	pi = new double*[numStates];
	opi = new double*[numStates];
	for (i = 0; i < numStates; i++) {
		kappa[1-me][i] = new int[A[1-me]];
		kappa[me][i] = new int[A[me]];
		pi[i] = new double[A[me]];
		opi[i] = new double[A[1-me]];
		for (j = 0; j < A[1-me]; j++) {
			kappa[1-me][i][j] = 0;
			opi[i][j] = 1.0 / A[1-me];
		}
		for (j = 0; j < A[me]; j++) {
			kappa[me][i][j] = 0;
			pi[i][j] = 1.0 / A[me];
		}
	}

	//printf("initialized kappa, pi, and opi\n");
	//fflush(stdout);	
	
	values = new double*[numStates+1];
	for (i = 0; i < numStates+1; i++) {
		values[i] = new double[A[me]];
		for (j = 0; j < A[me]; j++)
			values[i][j] = 0.0;
	}
	initOptimisticState();
	
	//printf("initialized values and optimisticState\n");
	//fflush(stdout);
	
	currentHistory = new int*[omega];
	dummyHist = new int*[omega];
	for (i = 0; i < omega; i++) {
		currentHistory[i] = new int[NUM_PLAYERS];
		dummyHist[i] = new int[NUM_PLAYERS];
		for (j = 0; j < NUM_PLAYERS; j++)
			currentHistory[i][j] = -1;
	}
	
	int num, ja;
	stateHistories = new int**[numStates];
	for (i = 0; i < numStates; i++) {
		num = i;
		stateHistories[i] = new int*[omega];
		for (j = 0; j < omega; j++) {
			stateHistories[i][j] = new int[NUM_PLAYERS];
			ja = getJointAction(num, j);
			num /= numJointActions;
			stateHistories[i][j][0] = ja / A[1];
			stateHistories[i][j][1] = ja % A[1];
		}
	}

	//printf("initialized histories stuff\n\n");
	//fflush(stdout);
	
	estado = -1;
	t = 0;
}

Rmax::~Rmax() {
	int i, j;
	for (i = 0; i < numStates; i++) {
		delete kappa[0][i];
		delete kappa[1][i];
		delete pi[i];
		delete opi[i];
		
		for (j = 0; j < omega; j++)
			delete stateHistories[i][j];
		delete stateHistories[i];
	}
	
	delete kappa[0];
	delete kappa[1];
	delete kappa;
	delete pi;
	delete opi;
	delete stateHistories;
		
	for (i = 0; i < omega; i++) {
		delete currentHistory[i];
		delete dummyHist[i];
	}
	delete currentHistory;
	delete dummyHist;
		
	for (i = 0; i < numStates + 1; i++)
		delete values[i];
	delete values;
}

int Rmax::move() {
	double expl = 1.0 / (10.0 + (t / 10.0));
	double num = (double)(rand()) / RAND_MAX;
	if ((num < expl) || (estado < 0)) {
		printf("r");
		return rand() % A[me];
	}
	
	return selectAction(estado);
}

int Rmax::moveGreedy() {
    if (estado < 0)
        return rand() % A[me];
    	
	return selectAction(estado);
}


void Rmax::update(int *acts) {
	int sprime = updateCurrentHistory(acts);
	if (estado >= 0) {
		kappa[1-me][estado][acts[1-me]] ++;
		kappa[me][estado][acts[me]] ++;
		int j, sum = 0;		
		for (j = 0; j < A[1-me]; j++)
			sum += kappa[1-me][estado][j];
		for (j = 0; j < A[1-me]; j++)
			opi[estado][j] = (double)(kappa[1-me][estado][j]) / (double)sum;
		
		bestResponse(M, opi);
	}	
	
	estado = sprime;
	t++;
}

int Rmax::selectAction(int s) {
	double num = (double)(rand()) / (double)(RAND_MAX);
	int i;
	double sum = 0.0;
	for (i = 0; i < A[me]; i++) {
		sum += pi[s][i];
		if (num < sum)
			break;
	}
	if (i == A[me])
		i --;
	
	return i;
}

int Rmax::updateCurrentHistory(int _actions[NUM_PLAYERS]) {
	int i;
	
	for (i = 0; i < (omega-1); i++) {
		currentHistory[i][0] = currentHistory[i+1][0];
		currentHistory[i][1] = currentHistory[i+1][1];
	}
	currentHistory[omega-1][0] = _actions[0];
	currentHistory[omega-1][1] = _actions[1];
	
	return stateFromHistory(currentHistory);
}

int Rmax::state(int **hist, int a0, int a1) {
	int i;
	
	for (i = 0; i < (omega-1); i++) {
		dummyHist[i][0] = hist[i+1][0];
		dummyHist[i][1] = hist[i+1][1];
	}
	dummyHist[omega-1][0] = a0;
	dummyHist[omega-1][1] = a1;
	
	return stateFromHistory(dummyHist);	
}

int Rmax::stateFromHistory(int **hist) {
	if (hist[0][0] < 0)
		return -1;
	
	int val, num = 0, i;
	for (i = 0; i < omega; i++) {
		val = A[1] * hist[i][0] + hist[i][1];
		//printf("move %i was joint action %i\n", i, val);
		num += val * pow(numJointActions, i);
	}

	return num;
}

int Rmax::getJointAction(int num, int index) {
	int val = pow(numJointActions, index+1);
	int ja = num % val;
	
	return ja;
}

void Rmax::bestResponse(double ***M, double **opi) {
	int i = 0;
	double change = 99999;
	while (change > 0.000001)
		change = greedyValueIteration(M, opi);

	int s, j;
	
	//printf("\nFor player %i:\n", me);
	for (s = 0; s < numStates; s++) {
		//printf("%i: ", s);
		for (j = 0; j < A[me]; j++) {
			pi[s][j] = 0.0;
			//printf("%.3lf (%i)\t", values[s][j], kappa[me][s][j]);
		}
		//printf("\n");
		
		pi[s][argmax(s)] = 1.0;		
	}
}

double Rmax::greedyValueIteration(double ***M, double **opi) {
	int s, sprime;
	int a, i, j;
	double nv, change = 0.0;
	int hist[MAXLENGTH][NUM_PLAYERS];
	
	for (i = 0; i < MAXLENGTH; i++) {
		for (j = 0; j < NUM_PLAYERS; j++) {
			hist[i][j] = 0;
		}
	}
    
	for (s = 0; s < numStates; s++) {
		for (a = 0; a < A[me]; a++) {
			if (kappa[me][s][a] < K)
				nv = maxV(numStates);
			else {
				nv = 0.0;
				for (j = 0; j < A[1-me]; j++) {
					if (me == 0)
						sprime = state(stateHistories[s], a, j);
					else
						sprime = state(stateHistories[s], j, a);
					if (me == 0)
						nv += opi[s][j] * (M[me][a][j] + gamma * maxV(sprime));
					else
						nv += opi[s][j] * (M[me][j][a] + gamma * maxV(sprime));
				}
			}
			change += fabs(values[s][a] - nv);
			values[s][a] = nv;
		}
	}
	
	return change;
}

double Rmax::maxV(int s) {
	int max = 0;
	int i;
	
	for (i = 1; i < A[me]; i++) {
		if (values[s][max] < values[s][i])
			max = i;
	}
	
	return values[s][max];
}

int Rmax::argmax(int s) {
	int max = 0;
	int i;
	
	for (i = 1; i < A[me]; i++) {
		if ((values[s][max] < values[s][i]) || ((values[s][max] == values[s][i]) && (rand() % 2)))
			max = i;
	}
	
	return max;
}


void Rmax::initOptimisticState() {
	int i, j;
	double mx = -99999;
	
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {			
			if (M[me][i][j] > mx)
				mx = M[me][i][j];
		}
	}
	
	for (i = 0; i < A[me]; i++) {
		values[numStates][i] = mx / (1.0 - gamma);
	}
}

void Rmax::printV() {
	int s, j;
	
	printf("\nFor player %i:\n", me);
	for (s = 0; s < numStates; s++) {
		printf("%i: ", s);
		for (j = 0; j < A[me]; j++) {
			printf("%.3lf (%i, %i)\t", values[s][j] * (1.0 - gamma), kappa[me][s][j], kappa[1-me][s][j]);
		}
		printf("\n");
	}
	
}

