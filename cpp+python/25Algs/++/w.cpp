#include "w.h"

W::W() {
    printf("incomplete W constructor\n");
    exit(1);
}


W::W(const char *nombre, int _omega, int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda) {
    omega = _omega;
    me = _me;
	int i, j;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
    
    cycled = false;
    estado = experto = -1;
    t = 0;
    
    printf("before deriving strategies\n");
    fflush(stdout);
    
    deriveStrategies();

    if (!strcmp(nombre, "aw"))
        learner = new a(_me, _A, _M, _lambda, numMyExperts);
    else if (!strcmp(nombre, "exp3w") || !strcmp(nombre, "exp3"))
        learner = new Exp3(me, (int)_lambda, 0.99, numMyExperts);
    else if (!strcmp(nombre, "eeew"))
        learner = new eee(_me, _lambda, numMyExperts);
    else if (!strcmp(nombre, "ucbw"))
        learner = new ucb(_me, _lambda, numMyExperts);
    else {
        printf("expert learner not found\n");
        exit(1);
    }
    
    learner->aspiration = -99999;
	for (i = 0; i < _A[0]; i++) {
		for (j = 0; j < _A[1]; j++) {
			if (_M[me][i][j] > learner->aspiration)
				learner->aspiration = _M[me][i][j];
		}
	}    
    
    br = new BR(me, A, M);
}

void W::deriveStrategies() {
	numStates = A[0] * A[1];
	getPotentialPolicies();
    
    beenThere = new bool[numStates];
    
	int numMyCombos = pow(numMyPolicies, numStates);

    resetCycle();

    if ((numMyCombos <= MAX_EXPERTS) && (numMyCombos > 0)) {
        numMyExperts = determineNumExperts(numMyCombos);
        deriveAllStrategies(numMyCombos);
        fullSet = true;
    }
    else {
        numMyExperts = deriveRandomStrategies();
        fullSet = false;
    }
    
    satisficingExperts = new bool[numMyExperts];
    int i;
    for (i = 0; i < numMyExperts; i++)
        satisficingExperts[i] = true;
}

void W::deriveAllStrategies(int numMyCombos) {
    int i, j, k, val1, val2, num;
    
	theStrats = new omegaStrategy*[numMyExperts];
	int *strats = new int[numStates];
	for (i = 0; i < numMyCombos; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(numMyPolicies, j);
			num = i % val1;
			num = i - num;
			val2 = pow(numMyPolicies, j+1);
			num = num % val2;
			strats[j] = num / val1;
		}
		
		theStrats[i] = new omegaStrategy(me, A, M, 1);
		for (j = 0; j < numStates; j++) {
			for (k = 0; k < A[me]; k++) {
				theStrats[i]->pi[j][k] = myPolicies[strats[j]][k];
			}
		}
	}
	delete strats;
	
	if (numMyExperts > numMyCombos)
		addMaximin();
    
}

int W::deriveRandomStrategies() {
    int i, j, k;
    bool present;
    
    printf("deriving random strategies\n");
    fflush(stdout);
    
    int **strats = new int*[MAX_EXPERTS];    
    for (i = 0; i < MAX_EXPERTS; i++) {
        strats[i] = new int[numStates];
        present = true;
        
        while (present) {
            for (j = 0; j < numStates; j++)
                strats[i][j] = rand() % numMyPolicies;
            
            present = findMatch(i, strats);
        }
    }
    
    int numStrategies = MAX_EXPERTS + 2;
    
   	theStrats = new omegaStrategy*[numStrategies];
    for (i = 0; i < MAX_EXPERTS; i++) {
		theStrats[i] = new omegaStrategy(me, A, M, 1);
		for (j = 0; j < numStates; j++) {
			for (k = 0; k < A[me]; k++) {
				theStrats[i]->pi[j][k] = myPolicies[strats[i][j]][k];
			}
		}
    }
    
    // add a strategy that is the BR strategy
    theStrats[MAX_EXPERTS] = new omegaStrategy(me, A, M, 1);
    for (j = 0; j < numStates; j++) {
        for (k = 0; k < A[me]; k++) {
            theStrats[MAX_EXPERTS]->pi[j][k] = -1.0;
        }
    }
    
    // add the maximin strategy if it isn't already there
    computeMaximin();
    printf("maximin (%.3lf): ", mnmx->mv);
    for (i = 0; i < A[me]; i++)
        printf("%.3lf ", mnmx->ms[i]);
    printf("\n");

    bool alreadyThere = false, same;
    for (i = 0; i < MAX_EXPERTS; i++) {
        same = true;
        for (j = 0; j < numStates; j++) {
            if (!isSame(theStrats[i]->pi[j], mnmx->ms)) {
                same = false;
                break;
            }
        }
        
        if (same) {
            alreadyThere = true;
            break;
        }
    }
    
    if (alreadyThere) {
        printf("%i: don't need to add maximin\n", me);
        numStrategies --;
        delete theStrats[MAX_EXPERTS+1];
    }
    else {
        printf("%i: need to add maximin\n", me);
        theStrats[MAX_EXPERTS+1] = new omegaStrategy(me, A, M, 1);
        for (j = 0; j < numStates; j++) {
            for (i = 0; i < A[me]; i++)
                theStrats[MAX_EXPERTS+1]->pi[j][i] = mnmx->ms[i];
        }        
    }

    for (i = 0; i < MAX_EXPERTS; i++)
        delete strats[i];
    delete strats;    
    
    return numStrategies;
}

bool W::findMatch(int index, int **strats) {
    int i, j;
    bool same;
    
    for (i = 0; i < index; i++) {
        same = true;
        for (j = 0; j < numStates; j++) {
            if (strats[i][j] != strats[index][j]) {
                same = false;
                break;
            }
        }
        
        if (same)
            return true;
    }
    
    return false;
}

W::~W() {
	int i;
	
	for (i = 0; i < numMyPolicies; i++)
		delete myPolicies[i];
	delete myPolicies;
	
	for (i = 0; i < numMyExperts; i++)
		delete theStrats[i];
	delete theStrats;
	
	delete beenThere;
	delete mnmx;
    
    delete br;
}

int W::move() {
    if (estado < 0) {
        //experto = learner->select(satisficingExperts);
        return rand() % A[me];
    }

    if (cycled) {
        //printf("i know you cycled\n");
        resetCycle();
        experto = learner->select(satisficingExperts);
        cycled = false;
        //printf("%i chose learner %i\n", me, experto);
    }

    return theStrats[experto]->selectAction(estado);
}

void W::update(int *acts) {
    estado = theStrats[0]->encodeJointAction(acts);

    if (!fullSet) {
        br->update(acts);
        
        // compute the best response strategy
        int i, j;
        for (i = 0; i < numStates; i++) {
            for (j = 0; j < A[me]; j++) {
                theStrats[MAX_EXPERTS]->pi[i][j] = 0.0;
            }
            theStrats[MAX_EXPERTS]->pi[i][br->myBR->ares->argmax(i)] = 1.0;
        }
    }    
    
    //printf("update %i\n", me);
    if (experto < 0) {
        //printf("  reset cycle\n");
        cycled = true;
        t++;
        return;
    }
        
    if (omega < 0) {
        if (tau == (numStates-1))
            cycled = true;
    }
    else {
        if (beenThere[estado] || (omega == 0))
            cycled = true;
    }
    
    beenThere[estado] = true;
	
	t++;
	tau ++;
    
	R += M[me][acts[0]][acts[1]];
	
	if (cycled)
		learner->update(R / tau, tau);
}

void W::resetCycle() {
	tau = 0;
	R = 0.0;
	for (int i = 0; i < numStates; i++)
		beenThere[i] = false;
	
	if (estado >= 0) {
        fflush(stdout);
		beenThere[estado] = true;
    }
}


void W::getPotentialPolicies() {
	int i, j;
	
    numMyPolicies = A[me];
    
	myPolicies = new double*[numMyPolicies];
	for (i = 0; i < numMyPolicies; i++) {
		myPolicies[i] = new double[A[me]];
        
        for (j = 0; j < A[me]; j++)
            myPolicies[i][j] = 0.0;
        myPolicies[i][i] = 1.0;
	}
}

bool W::isPure(double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if ((v[i] > 0.000001) && (v[i] < 0.999999))
			return false;
	}
	return true;
}

bool W::isSame(double *u, double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if (u[i] != v[i])
			return false;
	}
	
	return true;
}

void W::computeMaximin() {
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

void W::addMaximin() {
	int i, j;
	
	theStrats[numMyExperts-1] = new omegaStrategy(me, A, M, 1);
	for (j = 0; j < numStates; j++) {
		for (i = 0; i < A[me]; i++)
			theStrats[numMyExperts-1]->pi[j][i] = mnmx->ms[i];
	}
}

int W::determineNumExperts(int numCombos) {
    //printf("in determineNumExperts()\n");
    //fflush(stdout);
	computeMaximin();
    
    //printf("computed the maximin\n");
    //fflush(stdout);

	if (numMyPolicies == A[me]) {  // attack strat is pure
		if (isPure(mnmx->ms))
			return numCombos;
		else
			return numCombos + 1;
	}
	else {						 // attack strat is mixed
		printf("check if it is the same ... ");
		for (int i = 0; i < A[me]; i++)
			printf("(%.3lf vs %.3lf)   ", mnmx->ms[i], myPolicies[A[me]][i]);
		if (isSame(mnmx->ms, myPolicies[A[me]])) {
			printf("yes!\n");
			return numCombos;
		}
		else {
			printf("not\n");			
			return numCombos + 1;
		}
	}
}
