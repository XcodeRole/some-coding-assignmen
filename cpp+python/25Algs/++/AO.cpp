#include "AO.h"

AO::AO() {
	printf("entered into incomplete AO constructor\n");
	exit(1);
}

AO::AO(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda) {
	int i;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
	lambda = _lambda;
	
	deriveStrategies();
	
	estado = -1;
	t = 0;	
	
	v = 0.0;
	currentStrategy = NULL;

	omega = 0;

	evaluateChange = false;
}
	
void AO::deriveStrategies() {
	int i, j, k, val1, val2, num;
	
	numStates = A[0] * A[1];
	getPotentialPolicies();
	int numCombos = pow(numPolicies, numStates);
	numStrats = determineNumStrats(numCombos);
	
	printf("numStrats = %i; numCombos = %i\n", numStrats, numCombos);
	
	beenThere = new bool[numStates];
	
	theStrats = new omegaStrategy*[numStrats];
	int *strats = new int[numStates];
	
	//printf("strats:\n");
	for (i = 0; i < numCombos; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(numPolicies, j);
			num = i % val1;
			num = i - num;
			val2 = pow(numPolicies, j+1);
			num = num % val2;
			strats[j] = num / val1;
		}
		
		theStrats[i] = new omegaStrategy(me, A, 1);
		//printf("%i: ", i);
		for (j = 0; j < numStates; j++) {
			//	printf("<");
			for (k = 0; k < A[me]; k++) {
				theStrats[i]->pi[j][k] = policies[strats[j]][k];
				//		printf("%.3lf ", theStrats[i]->pi[j][k]);
			}
			//	printf("> ");
		}
		//printf("\n");
	}
	delete strats;
	
	if (numStrats > numCombos)
		addMaximin();
}

AO::~AO() {
	int i;
	
	for (i = 0; i < numPolicies; i++)
		delete policies[i];
	delete policies;
	
	for (i = 0; i < numStrats; i++)
		delete theStrats[i];
	delete theStrats;
	
	delete beenThere;
	delete mnmx;
}

int AO::move() {
	if (t == 0) {
		resetUtils();
		return rand() % A[me];
	}
	else if (t == 1) {
		setAspiration0();
		int ind = selectStrategy();
		currentStrategy = theStrats[ind];
		resetUtils();
	}
	else {
		double rho = computeInertia();
		double num = (double)(rand()) / RAND_MAX;
		if ((num > rho) && evaluateChange) {
			int ind = selectStrategy();
			currentStrategy = theStrats[ind];
		}
	}
	
	if (currentStrategy == NULL) {
		printf("NULL strategy\n");
		exit(1);
	}
    
	
	return currentStrategy->selectAction(estado);
}

void AO::update(int *acts) {
	estado = theStrats[0]->encodeJointAction(acts);
	if (estado >= 0) {
		if (beenThere[estado])
			evaluateChange = true;
		else
			evaluateChange = false;
		
		beenThere[estado] = true;
	}
	
	t++;
	
	sCuenta ++;
	v += M[me][acts[0]][acts[1]];
	
	if (evaluateChange) {
		for (int i = 0; i < sCuenta; i++)
			aspiration = lambda * (v / sCuenta) + (1.0 - lambda) * aspiration;
	}
}

void AO::resetUtils() {
	sCuenta = 0;
	v = 0.0;
	for (int i = 0; i < numStates; i++)
		beenThere[i] = false;
	
	if (estado >= 0)
		beenThere[estado] = true;
}

double AO::computeInertia() {
	if (!evaluateChange)
		return 1.0;
	
	double rho = (v / sCuenta) / aspiration;
	if (rho > 1.0)
		rho = 1.0;
	
	rho = pow(rho, sCuenta);
	
	resetUtils();
	
	return rho;
}

int AO::selectStrategy() {
	int strat = rand() % numStrats;
	//printf("chose %i\n", strat);
	//fflush(stdout);
	
	return strat;
}

void AO::setAspiration0() {
	int i, j;
	double high = -99999;
	
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			if (M[me][i][j] > high)
				high = M[me][i][j];
		}
	}
	
	aspiration = high;
	printf("%i: initial aspiration level = %.2lf\n", me, aspiration);
}

void AO::getPotentialPolicies() {
	int i, j;
	
	// i miminize his maximum
	minimaxLog *mnmx = new minimaxLog(A[me]);
	double *payoff = new double[numStates];
	int count = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[count] = -M[1-me][i][j];
			count ++;
		}
	}
	mnmx->getMinimax(A, me, payoff);
	delete payoff;
	
	//if (isPure(mnmx->ms)) {
		numPolicies = A[me];
	//}
	//else {
	//	numPolicies = A[me] + 1;
	//}
	
	policies = new double*[numPolicies];
	for (i = 0; i < numPolicies; i++) {
		policies[i] = new double[A[me]];
		if (i == A[me]) {
			for (j = 0; j < A[me]; j++)
				policies[i][j] = mnmx->ms[j];
		}
		else {
			for (j = 0; j < A[me]; j++)
				policies[i][j] = 0.0;
			policies[i][i] = 1.0;
		}
	}
	
/*	printf("\n%i: Possible Policies:\n", me);
	for (i = 0; i < numPolicies; i++) {
		for (j = 0; j < A[me]; j++) {
			printf("%.3lf  ", policies[i][j]);
		}
		printf("\n");
	}
	printf("\n");
*/	
	delete mnmx;
}

bool AO::isPure(double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if ((v[i] > 0.0) && (v[i] < 1.0))
			return false;
	}
	return true;
}

bool AO::isSame(double *u, double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if (u[i] != v[i])
			return false;
	}
	
	return true;
}

void AO::computeMaximin() {
	int i, j;
	
	mnmx = new minimaxLog(A[me]);
	double *payoff = new double[numStates];
	int count = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[count] = M[me][i][j];
			count ++;
		}
	}
	mnmx->getMinimax(A, me, payoff);
	delete payoff;
}

void AO::addMaximin() {
	int i, j;
	
	theStrats[numStrats-1] = new omegaStrategy(me, A, 1);
	for (j = 0; j < numStates; j++) {
		for (i = 0; i < A[me]; i++)
			theStrats[numStrats-1]->pi[j][i] = mnmx->ms[i];
	}
}

int AO::determineNumStrats(int numCombos) {
	computeMaximin();
	if (numPolicies == A[me]) {  // attack strat is pure
		if (isPure(mnmx->ms))
			return numCombos;
		else
			return numCombos + 1;
	}
	else {						 // attack strat is mixed
		printf("check if it is the same ... ");
		for (int i = 0; i < A[me]; i++)
			printf("(%.3lf vs %.3lf)   ", mnmx->ms[i], policies[A[me]][i]);
		if (isSame(mnmx->ms, policies[A[me]])) {
			printf("yes!\n");
			return numCombos;
		}
		else {
			printf("not\n");			
			return numCombos + 1;
		}
	}
}
