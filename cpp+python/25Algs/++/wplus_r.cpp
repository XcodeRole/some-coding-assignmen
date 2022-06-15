#include "wplus_r.h"

extern void Littman(Cell pago[NumDecisionPointsMax][NumDecisionPointsMax], int c[2], int a1[2], int a2[2], int _numDPA);

Wplus_r::Wplus_r() {
    printf("incomplete W constructor\n");
    exit(1);
}

Wplus_r::Wplus_r(const char *nombre, int _omega, int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda) {
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
        printf("Wplus_r: expert learner not found\n");
        exit(1);
    }
    
    im = new iModel(me, A, 1);
    br = new BR(me, A, M);
    
    setAspiration0();
}

void Wplus_r::deriveStrategies() {
	int i;
	
	numStates = A[0] * A[1];
	getPotentialPolicies();
	numTeachingStrats = numIStrats = pow(numMyPolicies, numStates);
    
	beenThere = new bool[numStates];
    
    if ((numTeachingStrats <= MAX_EXPERTS) && (numTeachingStrats > 0)) {
        printf("should have selected wplus\n");
        exit(1);
    }
    else {
        numIStrats = MAX_EXPERTS;
        numTeachingStrats = numMyExperts = deriveRandomStrategies();
    }
    
    satisficingExperts = new bool[numMyExperts];
    for (i = 0; i < numMyExperts; i++)
        satisficingExperts[i] = true;	
}

int Wplus_r::deriveRandomStrategies() {
    int i, j, k;
    bool present;
    
    printf("deriving random strategies\n");
    fflush(stdout);
    
    // determine my teacher strategies
    int numStrategies = MAX_EXPERTS;// + 2;
    theTeachingStrats = new omegaPair*[numStrategies];
    
    int **myStrats = new int*[MAX_EXPERTS];
    for (i = 0; i < MAX_EXPERTS; i++) {
        myStrats[i] = new int[numStates];
        present = true;
        
        while (present) {
            for (j = 0; j < numStates; j++)
                myStrats[i][j] = rand() % numMyPolicies;
            
            present = findMatch(i, myStrats);
        }
        
        theTeachingStrats[i] = new omegaPair(me, A, M, 1, FOLLOWER, myStrats[i]);
    }

    // determine his teacher strategies
    theIStrats = new omegaPair**[MAX_EXPERTS];
    int **hisStrats = new int*[MAX_EXPERTS];
    for (i = 0; i < MAX_EXPERTS; i++) {
        //printf("expert %i: ", i);
        //fflush(stdout);
        hisStrats[i] = new int[numStates];
        present = true;        
        while (present) {
            for (j = 0; j < numStates; j++)
                hisStrats[i][j] = rand() % A[1-me];
            
            present = findMatch(i, hisStrats);
        }
        theIStrats[i] = new omegaPair*[numStates];
        for (j = 0; j < numStates; j++) {
            //printf("%i ", j);
            //fflush(stdout);
            theIStrats[i][j] = new omegaPair(me, A, M, hisStrats[i], myStrats, j);
        }
        //printf("\n");
    }
    
    
    for (i = 0; i < MAX_EXPERTS; i++) {
        delete myStrats[i];
        delete hisStrats[i];
    }
    delete myStrats;
    delete hisStrats;
    
    computeMaximin();
/*    
     // add a strategy that is the BR strategy
    theTeachingStrats[MAX_EXPERTS] = new omegaStrategy(me, A, M, 1);
    for (j = 0; j < numStates; j++) {
        for (k = 0; k < A[me]; k++) {
            theTeachingStrat[MAX_EXPERTS]->pi[j][k] = -1.0;
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
            if (!isSame(theTeachingStrats[i]->pi[j], mnmx->ms)) {
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
        delete theTeachingStrats[MAX_EXPERTS+1];
    }
    else {
        printf("%i: need to add maximin\n", me);
        theTeachingStrats[MAX_EXPERTS+1] = new omegaStrategy(me, A, M, 1);
        for (j = 0; j < numStates; j++) {
            for (i = 0; i < A[me]; i++)
                theTeachingStrats[MAX_EXPERTS+1]->pi[j][i] = mnmx->ms[i];
        }        
    }
*/
    //exit(1);
    
	theStrats = new omegaStrategy*[numStrategies];
	for (i = 0; i < numStrategies; i++)
		theStrats[i] = theTeachingStrats[i]->ares;    
    
    return numStrategies;
}

bool Wplus_r::findMatch(int index, int **strats) {
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

Wplus_r::~Wplus_r() {
    printf("%i: loss = %lf\n", me, ((mnmx->mv * t) - mu));
    
	int i, j;
	
	for (i = 0; i < numMyPolicies; i++)
		delete myPolicies[i];
	delete myPolicies;
	for (i = 0; i < numHisPolicies; i++)
		delete hisPolicies[i];
	delete hisPolicies;
	
	for (i = 0; i < numTeachingStrats; i++)
		delete theTeachingStrats[i];
	for (i = 0; i < numIStrats; i++) {
        for (j = 0; j < numStates; j++) {
            delete theIStrats[i][j];
        }
        delete theIStrats[i];
    }
	delete theTeachingStrats;
	delete theIStrats;
	delete im;
	delete br;
	delete beenThere;
	delete satisficingExperts;
}

int Wplus_r::move() {
    if (estado < 0) {
        return rand() % A[me];
    }
    
    if (cycled) {
        resetCycle();
        determineSatisficingExperts();
        experto = learner->select(satisficingExperts);
        cycled = false;
    }
    
    return theStrats[experto]->selectAction(estado);
}

void Wplus_r::determineSatisficingExperts() {
    //if (override())
    //    return;
    
    if (beSelective)
        determineSatisficing();
    else {
        int i;
        
        for (i = 0; i < numMyExperts; i++)
            satisficingExperts[i] = true;
    }
}

void Wplus_r::update(int *acts) {
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

void Wplus_r::resetCycle() {
	tau = 0;
	R = 0.0;
	for (int i = 0; i < numStates; i++)
		beenThere[i] = false;
	
	if (estado >= 0)
		beenThere[estado] = true;
}

void Wplus_r::getPotentialPolicies() {
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

bool Wplus_r::isPure(double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if ((v[i] > 0.000001) && (v[i] < 0.999999))
			return false;
	}
	return true;
}

bool Wplus_r::isSame(double *u, double *v) {
	int i;
	for (i = 0; i < A[me]; i++) {
		if (u[i] != v[i])
			return false;
	}
	
	return true;
}

void Wplus_r::computeMaximin() {
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

void Wplus_r::addMaximin() {
	int i, j;
	
	theStrats[numMyExperts-1] = new omegaStrategy(me, A, M, 1);
	for (j = 0; j < numStates; j++) {
		for (i = 0; i < A[me]; i++)
			theStrats[numMyExperts-1]->pi[j][i] = mnmx->ms[i];
	}
}

int Wplus_r::determineNumExperts(int numCombos) {
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


int Wplus_r::setAspiration0() {
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
	if (high <= mnmx->mv) {
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
	
    //	s = findSteadyState(theTeachingStrats[index]);
    //	return s;
    return -1;
}

int Wplus_r::findSteadyState(omegaPair *oPair, int s) {
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

/*int Wplus_r::whichOneIsIt(omegaStrategy *strat) {
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

int Wplus_r::whichOneIsIt(omegaStrategy *strat) {
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

bool Wplus_r::override() {
    int i;
    for (i = 0; i < numMyExperts; i++)
        satisficingExperts[i] = false;
/*    
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
*/	
	return false;
}


void Wplus_r::determineSatisficing() {
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
		if ((theIStrats[i][estado]->ares->Valor[estado] >= learner->aspiration) && (im->match(theIStrats[i][estado]->thars))) {
			int ind = whichOneIsIt(theIStrats[i][estado]->ares);
			if (!satisficingExperts[ind]) {
				numviable ++;
                satisficingExperts[ind] = true;
                //printf("%i(%i) ", ind, i);
            }
		}
	}
/*	
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
*/    
    //printf("%i: numviable = %i\n", me, numviable);
}

double Wplus_r::NB() {
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
