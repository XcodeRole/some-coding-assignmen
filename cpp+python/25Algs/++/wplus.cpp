#include "wplus.h"

extern void Littman(Cell pago[NumDecisionPointsMax][NumDecisionPointsMax], int c[2], int a1[2], int a2[2], int _numDPA);

Wplus::Wplus() {
    printf("incomplete W constructor\n");
    exit(1);
}

Wplus::Wplus(const char *nombre, int _omega, int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda) {
    omega = _omega;
    me = _me;
    lambda = _lambda;
    
	int i;
	
	me = _me;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	M = _M;
    
    estado = experto = -1;
    t = 0;

    deriveStrategies();

    if (!strcmp(nombre, "aw++"))
        learner = new a(_me, _A, _M, _lambda, numMyExperts);
    else if (!strcmp(nombre, "exp3w++"))
        learner = new Exp3(me, (int)_lambda, 0.99, numMyExperts);
    else if (!strcmp(nombre, "eeew++"))
        learner = new eee(me, _lambda, numMyExperts);
    else if (!strcmp(nombre, "ucbw++"))
        learner = new ucb(me, _lambda, numMyExperts);
    else {
        printf("wplus: expert learner not found\n");
        exit(1);
    }
    
    im = new iModel(me, A, 1);
    br = new BR(me, A, M);
    
    setAspiration0();
}

void Wplus::deriveStrategies() {
	int i;
	
	numStates = A[0] * A[1];
	getPotentialPolicies();
	numTeachingStrats = numIStrats = pow(numMyPolicies, numStates);
    
	beenThere = new bool[numStates];
    
    if ((numTeachingStrats <= MAX_EXPERTS) && (numTeachingStrats > 0)) {
        numMyExperts = determineNumExperts(numTeachingStrats);
        deriveAllStrategies();
    }
    else {
        printf("should have selected wplus_r\n");
        exit(1);
        //numMyExperts = deriveRandomStrategies();
        //fullSet = false;
    }

    satisficingExperts = new bool[numMyExperts];
    for (i = 0; i < numMyExperts; i++)
        satisficingExperts[i] = true;	
}

void Wplus::deriveAllStrategies() {
	int i, j, k, val1, val2, num, ind;
    
	theTeachingStrats = new omegaPair*[numTeachingStrats];
	
	double **strats = new double*[numStates];
	for (i = 0; i < numStates; i++)
		strats[i] = new double[A[me]];
	
    bool v = false;
    if (v)
        printf("teaching strats:\n");
	for (i = 0; i < numTeachingStrats; i++) {
        if (v)
            printf("%i: ", i);
		for (j = 0; j < numStates; j++) {
			val1 = pow(numMyPolicies, j);
			num = i % val1;
			num = i - num;
			val2 = pow(numMyPolicies, j+1);
			num = num % val2;
			
			ind = num / val1;
            
            if (v)
                printf("%i", ind);
            
			for (k = 0; k < A[me]; k++)
				strats[j][k] = myPolicies[ind][k];
		}
		
        //if (v)
        //if ((i % 100) == 0)        
        //    printf("init teaching strat %i of %i\n", i, numTeachingStrats);
		
        theTeachingStrats[i] = new omegaPair(me, A, M, 1, FOLLOWER, strats);
		
        if (v) {
            //printf("%i: ", i);
            //theTeachingStrats[i]->ares->printStrat();
            printf(" (");
            for (j = 0; j < numStates; j++)
                printf("%.2lf; ", theTeachingStrats[i]->ares->Valor[j]);
            printf(") vs ");
            //printf(" (%.2lf; %.2lf; %.2lf; %.2lf) vs ", theTeachingStrats[i]->ares->Valor[0], theTeachingStrats[i]->ares->Valor[1], theTeachingStrats[i]->ares->Valor[2], theTeachingStrats[i]->ares->Valor[3]);
            theTeachingStrats[i]->thars->printStrat3();
            printf("\n");
        }
    }
    
	for (i = 0; i < numStates; i++)
		delete strats[i];
	delete strats;
	
	strats = new double*[numStates];
	for (i = 0; i < numStates; i++)
		strats[i] = new double[A[1-me]];

    if (v)
        printf("follower strats:\n");    
	
    theIStrats = new omegaPair*[numIStrats];
	for (i = 0; i < numIStrats; i++) {
		for (j = 0; j < numStates; j++) {
			val1 = pow(numHisPolicies, j);
			num = i % val1;
			num = i - num;
			val2 = pow(numHisPolicies, j+1);
			num = num % val2;
            
			ind = num / val1;
			for (k = 0; k < A[1-me]; k++)
				strats[j][k] = hisPolicies[ind][k];
		}
        
        //if (v)
        //if ((i % 100) == 0)
        //    printf("init follower strat %i of %i\n", i, numIStrats);
		theIStrats[i] = new omegaPair(me, A, M, 1, TEACHER, strats);
        
        if (v) {
            printf("%i: ", i);
            theIStrats[i]->ares->printStrat();
            printf(" (%.2lf; %.2lf; %.2lf; %.2lf) vs ", theIStrats[i]->ares->Valor[0], theIStrats[i]->ares->Valor[1], theIStrats[i]->ares->Valor[2], theIStrats[i]->ares->Valor[3]);
            theIStrats[i]->thars->printStrat();
            printf("\n");
        }
	}
	
	for (i = 0; i < numStates; i++)
		delete strats[i];
	delete strats;
    
	theStrats = new omegaStrategy*[numMyExperts];
	for (i = 0; i < numTeachingStrats; i++)
		theStrats[i] = theTeachingStrats[i]->ares;
	
	if (numMyExperts > numTeachingStrats) {
		addMaximin();
        minimaxStrat = numTeachingStrats;
		printf("%i: maximin is strat %i\n", me, numTeachingStrats);
	}
	else {
		bool same;
		for (i = 0; i < numMyExperts; i++) {
			same = true;
			for (j = 0; j < numStates; j++) {
				if (!isSame(mnmx->ms, theStrats[i]->pi[j])) {
					same = false;
					break;
				}
			}
			if (same == true) {
                minimaxStrat = i;
				printf("maximin %.3lf: (%.3lf; %.3lf) is strat %i: ", mnmx->mv, mnmx->ms[0], mnmx->ms[1], i);
				theStrats[i]->printStrat();
				printf("\n");
				break;
			}
		}
	}
}


Wplus::~Wplus() {
    printf("%i: loss = %lf\n", me, ((mnmx->mv * t) - mu));
    
	int i;
	
	for (i = 0; i < numMyPolicies; i++)
		delete myPolicies[i];
	delete myPolicies;
	for (i = 0; i < numHisPolicies; i++)
		delete hisPolicies[i];
	delete hisPolicies;
	
	for (i = 0; i < numTeachingStrats; i++)
		delete theTeachingStrats[i];
	for (i = 0; i < numIStrats; i++)        
		delete theIStrats[i];
	delete theTeachingStrats;
	delete theIStrats;
	delete im;
	delete br;
	delete beenThere;
	delete satisficingExperts;
}

int Wplus::move() {
    //printf("(a%.2lf)", learner->aspiration);
    //fflush(stdout);
    if (estado < 0) {
        //experto = learner->select(satisficingExperts);
        return rand() % A[me];
    }

    if (cycled) {
        resetCycle();
        //printf("c");
        //fflush(stdout);
        determineSatisficingExperts();
        //printf("t");
        //fflush(stdout);
        experto = learner->select(satisficingExperts);
        //printf("i");
        //fflush(stdout);
        cycled = false;
        //printf("%i chose learner %i\n", me, experto);
    }
    
    //printf("(e%i)", experto);
    //fflush(stdout);

    return theStrats[experto]->selectAction(estado);
}

void Wplus::determineSatisficingExperts() {
    if (override())
        return;
    
    if (beSelective)
        determineSatisficing();
    else {
        int i;
        
        for (i = 0; i < numMyExperts; i++)
            satisficingExperts[i] = true;
    }
}

void Wplus::update(int *acts) {
	R += M[me][acts[0]][acts[1]];
    mu += M[me][acts[0]][acts[1]];

    if (experto < 0) {
        estado = theStrats[0]->encodeJointAction(acts);
        cycled = true;
        t++;
        return;
    }
    
    double betita = 1.0 / (theStrats[experto]->usageCount + 1);
    if (betita <= 2.0 * lambda)
        betita = 2.0 * lambda;
    theStrats[experto]->r = betita * M[me][acts[0]][acts[1]] + (1.0 - betita) * theStrats[experto]->r;
    theStrats[experto]->usageCount ++;        
    
    //printf("this ... ");
	br->update(acts);	
	if (estado >= 0)
		im->update(acts, estado, t);
    //printf("part?");
    
    //im->print();
    estado = theStrats[0]->encodeJointAction(acts);
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
    //printf("tau = %i; R = %.2lf\n", tau, R);
	
	if (cycled) {
		learner->update(R / tau, tau);
		if (((mnmx->mv * t) - mu) > permissibleLoss) {
            printf("%i: I'm losing!!!!! mu = %lf; mv*t = %lf; permissibleLoss = %lf\n", me, mu, mnmx->mv * t, permissibleLoss);
			alwaysMM = true;
        }
    }        
}

void Wplus::resetCycle() {
	tau = 0;
	R = 0.0;
	for (int i = 0; i < numStates; i++)
		beenThere[i] = false;
	
	if (estado >= 0)
		beenThere[estado] = true;
}

void Wplus::getPotentialPolicies() {
	int i, j;
	
    // Compute my policies
    
    numMyPolicies = A[me];

	myPolicies = new double*[numMyPolicies];
	for (i = 0; i < numMyPolicies; i++) {
		myPolicies[i] = new double[A[me]];
        for (j = 0; j < A[me]; j++)
            myPolicies[i][j] = 0.0;
        myPolicies[i][i] = 1.0;
	}
    
    printf("numMyPolicies = %i;\n", numMyPolicies);
    for (i = 0; i < numMyPolicies; i++) {
        for (j = 0; j < A[me]; j++)
            printf("%lf ", myPolicies[i][j]);
        printf("\n");
    }
    
    
    // Compute his policies
    numHisPolicies = A[1-me];
    
	hisPolicies = new double*[numHisPolicies];
	for (i = 0; i < numHisPolicies; i++) {
		hisPolicies[i] = new double[A[1-me]];
        for (j = 0; j < A[1-me]; j++)
            hisPolicies[i][j] = 0.0;
        hisPolicies[i][i] = 1.0;
	}
    
    printf("numHisPolicies = %i;\n", numHisPolicies);
    for (i = 0; i < numHisPolicies; i++) {
        for (j = 0; j < A[1-me]; j++)
            printf("%lf ", hisPolicies[i][j]);
        printf("\n");
    }
}

bool Wplus::isPure(double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if ((v[i] > 0.000001) && (v[i] < 0.999999))
			return false;
	}
	return true;
}

bool Wplus::isSame(double *u, double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if (u[i] != v[i])
			return false;
	}
	
	return true;
}

void Wplus::computeMaximin() {
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
/*    
    mnmx2 = new minimaxLog(A[1-me]);
	count = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			payoff[count] = M[1-me][i][j];
			count ++;
		}
	}
	mnmx2->getMinimax(A, 1-me, payoff);
*/
	delete payoff;
    
    printf("maximin %.3lf: (%.3lf; %.3lf)\n", mnmx->mv, mnmx->ms[0], mnmx->ms[1]);
//    printf("maximin2 %.3lf: (%.3lf; %.3lf)\n", mnmx2->mv, mnmx2->ms[0], mnmx2->ms[1]);
    
	alwaysMM = false;
	permissibleLoss = 40.0;
	mu = 0.0;    
}

void Wplus::addMaximin() {
	int i, j;
	
	theStrats[numMyExperts-1] = new omegaStrategy(me, A, M, 1);
	for (j = 0; j < numStates; j++) {
		for (i = 0; i < A[me]; i++)
			theStrats[numMyExperts-1]->pi[j][i] = mnmx->ms[i];
	}
}

int Wplus::determineNumExperts(int numCombos) {
	computeMaximin();
    if (isPure(mnmx->ms))
        return numCombos;
    else
        return numCombos + 1;
    
/*    
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
	}*/
}


int Wplus::setAspiration0() {
	int i, j, index = -1;
	double high = -99999, theMin;
	int s;
    
	for (i = 0; i < numTeachingStrats; i++) {
        theMin = 99999;
        for (j = 0; j < numStates; j++) {
            s = findSteadyState(theTeachingStrats[i], j);
            if (theMin > theTeachingStrats[i]->ares->Valor[s])
                theMin = theTeachingStrats[i]->ares->Valor[s];
        }

		if (high < theMin) {
            //printf("%i: enforced: %.3lf\n", i, theMin);
			high = theMin;
			index = i;
		}
        //printf("\n");
	}
	
    beSelective = true;
    
    // find where ray from (max, max) to (0, 0) entersects the convex hull of the payoff space
	if (high < mnmx->mv) {
        //high = NB();
		printf("no effective (w=1)-strategy\n");
        beSelective = false;
		for (i = 0; i < A[0]; i++) {
			for (j = 0; j < A[1]; j++) {
				if (M[me][i][j] > high)
					high = M[me][i][j];
			}
		}
	}
    
	learner->aspiration = high;
	printf("%i: initial aspiration level = %.3lf\n", me, learner->aspiration);
	
//	s = findSteadyState(theTeachingStrats[index]);
//	return s;
    return -1;
}

int Wplus::findSteadyState(omegaPair *oPair, int s) {
	int acts[2], i;
	bool *visited = new bool[numStates];
	for (i = 0; i < numStates; i++)
		visited[i] = false;
	
	while (!visited[s]) {
		visited[s] = true;
		acts[me] = oPair->ares->selectAction(s);
		acts[1-me] = oPair->thars->selectAction(s);
		s = oPair->ares->encodeJointAction(acts);
	}
	delete visited;
	
	return s;
}

/*int Wplus::whichOneIsIt(omegaStrategy *strat) {
	int i;
	
	//strat->printStrat();
	//printf("\n");
	
	for (i = 0; i < numTeachingStrats; i++) {
        //theStrats[i]->printStrat();
        //printf(" ");
		if (strat->matches(theStrats[i])) {
            //printf("\n");
			return i;
        }
	}
	
	printf("couldn't identify the strat: (%i)\n", i);
    strat->printStrat();
    printf("\n");
    for (i = 0; i < numStates; i++)
        printf("%.1lf ", strat->pi[i][0]);
	exit(1);
	
	return -1;
}*/

int Wplus::whichOneIsIt(omegaStrategy *strat) {
    int i, j, a;
    
    int val = 0;
    for (i = 0; i < numStates; i++) {
        a = 0;
        for (j = 1; j < A[me]; j++) {
            if (strat->pi[i][j] > strat->pi[i][a])
                a = j;
        }
    
        val += pow(A[me], i) * a;
    }
    
    return val;
}

bool Wplus::override() {
    int i;
    for (i = 0; i < numMyExperts; i++)
        satisficingExperts[i] = false;

	if (alwaysMM) {
        satisficingExperts[minimaxStrat] = true;
		printf("%i: trigger to minimaxStrat\n", me);
		//fflush(stdout);
		return true;
	}
	
	int index = whichOneIsIt(br->myBR->ares);
	if (br->myBR->ares->Valor[estado] >= learner->aspiration) {
		double val, highval = -99999;
		
		for (i = 0; i < numTeachingStrats; i++) {
			if (theTeachingStrats[i]->ares->Valor[estado] >= learner->aspiration) {
				if (theStrats[i]->usageCount == 0)
					val = 1.0;
				else
					val = theStrats[i]->r;
				if (val > highval)
					highval = val;
			}
		}
		
		for (i = 0; i < numIStrats; i++) {
			if ((theIStrats[i]->ares->Valor[estado] >= learner->aspiration)  && (im->match(theIStrats[i]->thars))) {
				int ind = whichOneIsIt(theIStrats[i]->ares);
				if (theStrats[ind]->usageCount == 0)
					val = 1.0;
				else
					val = theStrats[ind]->r;
				if (val > highval)
					highval = val;
			}
		}
		
		//printf("vs highval = %.3lf\n", highval);
		int index = whichOneIsIt(br->myBR->ares);
		if ((theStrats[index]->r >= highval) || (theStrats[index]->usageCount == 0)) {
            //printf("%i: force br; val = %lf; count = %i; highval = %lf\n", me, theStrats[index]->r, theStrats[index]->usageCount, highval);
            satisficingExperts[index] = true;
			return true;
		}
	}
	
	return false;
}


void Wplus::determineSatisficing() {
	int numviable = 0;
    int i;
    for (i = 0; i < numMyExperts; i++)
        satisficingExperts[i] = false;
    
	//im->print();
	//printf("viable: (t) ");
	for (i = 0; i < numTeachingStrats; i++) {
		if (theTeachingStrats[i]->ares->Valor[estado] >= learner->aspiration) {
			satisficingExperts[i] = true;
			numviable ++;
            //printf("%i ", i);
		}
	}
	
    //printf("(f) ");
	for (i = 0; i < numIStrats; i++) {
		if ((theIStrats[i]->ares->Valor[estado] >= learner->aspiration) && (im->match(theIStrats[i]->thars))) {
			int ind = whichOneIsIt(theIStrats[i]->ares);
			if (!satisficingExperts[ind]) {
				numviable ++;
                satisficingExperts[ind] = true;
                //printf("%i(%i) ", ind, i);
            }
		}
	}
	
   // printf("(br) ");
	// add br if it is satisficing
	int index = whichOneIsIt(br->myBR->ares);
	if ((br->myBR->ares->Valor[estado] >= learner->aspiration) || (numviable == 0)) {
		if (!satisficingExperts[index]) {
			numviable ++;
            satisficingExperts[index] = true;
            //printf("%i ", index);
        }
	}
    //printf("\n");
    
    //printf("%i: numviable = %i\n", me, numviable);
}

double Wplus::NB() {
	Cell Payoffs[NumDecisionPointsMax][NumDecisionPointsMax];
	int c[2], a1[2], a2[2];
	
	int i, j;
	double p1, p2;
	    
	int count = 0;
	for (i = 0; i < A[0]; i++) {
		for (j = 0; j < A[1]; j++) {
			Payoffs[i][j].r1 = M[me][i][j];
			Payoffs[i][j].r2 = M[1-me][i][j];
			count ++;
		}
	}
    
	Littman(Payoffs, c, a1, a2, A[me]);
    
	p1 = (Payoffs[a1[0]][a2[0]].r1 * c[0]) + (Payoffs[a1[1]][a2[1]].r1 * c[1]);
	p1 /= c[0] + c[1];
	p2 = (Payoffs[a1[0]][a2[0]].r2 * c[0]) + (Payoffs[a1[1]][a2[1]].r2 * c[1]);
	p2 /= c[0] + c[1];
	//bargain1 = p1;
	//bargain2 = p2;
	
	printf("NB = (%lf, %lf)\n", p1, p2);
    
    if (me == 0)
        return p1;
    else
        return p2;
}

