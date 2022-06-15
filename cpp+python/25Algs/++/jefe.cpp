#include "jefe.h"

jefe::jefe() {
    printf("incomplete jefe constructor\n");
    exit(1);
}


jefe::jefe(const char *nombre, int _me, int _A[2], double ***_M, double _lambda, char _game[1024]) {
    A[0] = _A[0];
    A[1] = _A[1];
    me = _me;
    M = _M;
    strcpy(game, _game);
    
    numStates = A[0] * A[1];
    t = 0;
    
    beenThere = new bool[numStates];
    
    estado = experto = -1;
    cycled = false;
    
    if (!strcmp(nombre, "exp3w_simp") || !strcmp(nombre, "eeew_simp") || !strcmp(nombre, "S_simp")) {
        printf("sencillo\n"); fflush(stdout);
        numExperts = determineExpertsSencillo();
        sencillo = true;
    }
    else {
        numExperts = determineExperts();
        sencillo = false;
    }
    printf("numExperts = %i\n", numExperts); fflush(stdout);
    
    cycleFull = 1;
    
    if (!strcmp(nombre, "S") || !strcmp(nombre, "S_simp")) {
        learner = new a(_me, _A, _M, _lambda, numExperts);
        cycleFull = 0;
    }
    else if (!strcmp(nombre, "exp3w") || !strcmp(nombre, "exp3") || !strcmp(nombre, "exp3w_simp"))
        learner = new Exp3(me, (int)_lambda, 0.99, numExperts);
    else if (!strcmp(nombre, "eeew") || !strcmp(nombre, "eeew_simp")) {
        learner = new eee(_me, _lambda, numExperts);
        cycleFull = 1;
    }
    else if (!strcmp(nombre, "ucbw"))
        learner = new ucb(_me, _lambda, numExperts);
    else {
        printf("expert learner not found\n");
        exit(1);
    }
    
    int i, j;
    
    learner->aspiration = -99999;
	for (i = 0; i < _A[0]; i++) {
		for (j = 0; j < _A[1]; j++) {
			if (_M[me][i][j] > learner->aspiration)
				learner->aspiration = _M[me][i][j];
		}
	}
    
//    re[0]->printTeacherFollower();
//    printf("\n");
//    re[0]->printFollowerTeacher();
//    printf("\n");

    if (me == 0) {
        char nmbr[1024];
        sprintf(nmbr, "../experts/log_%s.txt", game);
        fpLogExperts = fopen(nmbr, "w");
    }
}

int jefe::determineExpertsSencillo() {
    resetCycle();
    
    mnmx[0] = computeMaximin(0);
    mnmx[1] = computeMaximin(1);
    attack0 = computeAttack(0);
    attack1 = computeAttack(1);

    int numSolutionPairs = 0;
    int i, j;
    for (i = 1; i <= A[me]; i++)
        numSolutionPairs += i;
    printf("%i\n", numSolutionPairs);
    
    re = new REexpert*[numSolutionPairs];
    REcount = 0;
    for (i = 0; i < A[me]; i++) {
        for (j = i; j < A[me]; j++) {
            //printf("%i, %i -- ", i, j); fflush(stdout);
            re[REcount] = new REexpert(me, M, A, i, j);
            //printf("%i, %i\n", i, j); fflush(stdout);
            REcount ++;
        }
    }

    int numEs = REcount + 1;   // simple experts + maximin
    satisficingExperts = new bool[numEs];
    for (i = 0; i < numEs; i++)
        satisficingExperts[i] = true;
    
    return numEs;
}


/* *******************************
// 
//  Expert1 = maximin strategy
//  Expert2 = best response to the ficticious play assessment
//  Experts 3 to (REcount+2) = teaching strategies of RE
//  Experts (REcount + 3) to (2*REcount + 2) = follower strategies of RE
// 
 ********************************* */
int jefe::determineExperts() {
    resetCycle();

    // first I need to generate my teaching-follower pairs (REexperts)
    determineStrategyPairs();
    int numEs = REcount * 2 + 2;
    
    br = new Rmax(me, A, M, 1, 0, 0.95);
    
    int i;
    
    satisficingExperts = new bool[numEs];
    int ind;
    for (i = 0; i < numEs; i++) {
        if ((i < 2) || ((i % 2) == 1))
            satisficingExperts[i] = true;
        else {
            ind = (i - 2) / 2;
            if (re[ind]->enforceable) {
                satisficingExperts[i] = true;
                //printf("%i: there\n", i);
            }
            else {
                satisficingExperts[i] = false;
                //printf("%i: not there\n", i);
            }
        }
    }
    
    return numEs;
}

void jefe::determineStrategyPairs() {
    int i, j, k;

    int numSolutionPairs = 0;    
    for (i = 0; i < numStates; i++)
        numSolutionPairs += (i+1);

    SolutionPair *Theta = new SolutionPair[numSolutionPairs];;
    createSolutionPairs(Theta);
    mnmx[0] = computeMaximin(0);
    mnmx[1] = computeMaximin(1);
    attack0 = computeAttack(0);
    attack1 = computeAttack(1);
    
    REcount = 0;
    re = new REexpert*[numSolutionPairs];
    
    for (i = 0; i < numSolutionPairs; i++) {
        if ((Theta[i].one >= mnmx[0]->mv) && (Theta[i].one > 0) && (Theta[i].two >= mnmx[1]->mv) && (Theta[i].two > 0) ) {
            re[REcount] = new REexpert(me, M, A, Theta[i].s1, Theta[i].s2, attack0, attack1);

            REcount ++;
        }
    }
    
    delete[] Theta;
}

void jefe::createSolutionPairs(SolutionPair *Theta) {
    int i, j;
        
    int c = 0;
    for (i = 0; i < numStates; i++) {
        for (j = i; j < numStates; j++) {
            Theta[c].s1 = i;
            Theta[c].s2 = j;
            
            Theta[c].one = (pay(0, Theta[c].s1) + pay(0, Theta[c].s2)) / 2.0;
            Theta[c].two = (pay(1, Theta[c].s1) + pay(1, Theta[c].s2)) / 2.0;
            
            Theta[c].min = Theta[c].one;
            if (Theta[c].one > Theta[c].two)
                Theta[c].min = Theta[c].two;
            
            c++;
        }
    }
}


jefe::~jefe() {
    delete beenThere;
    delete learner;
    delete satisficingExperts;
    delete mnmx[0];
    delete mnmx[1];
    if (!sencillo)
        delete br;
    delete attack0;
    delete attack1;    

    int i;
    for (i = 0; i < REcount; i++)
        delete re[i];
    delete re;
}

int jefe::expertType(int index) {
    if (index < 2)
        return index; // maximin or BR
    
    if ((index % 2) == 0) {
        // bully, fair, bullied, or other
        if (re[(index - 2) / 2]->barR[0] > 0.9) {
            return 2;
        }
        else if ((re[(index - 2) / 2]->barR[0] < 0.85) && (re[(index - 2) / 2]->barR[0] > 0.83)) {
            return 3;
        }
        else if (re[(index - 2) / 2]->barR[0] < 0.6) {
            return 4;
        }
        else {
            //printf("%lf\n", re[(index - 2) / 2]->barR[0]);
            return 5;
        }
    }
    else {
        // follower
        // bully, fair, bullied, or other        
        if (re[(index - 2) / 2]->barR[0] > 0.9) {
            return 6;
        }
        else if ((re[(index - 2) / 2]->barR[0] < 0.85) && (re[(index - 2) / 2]->barR[0] > 0.83)) {
            return 7;
        }
        else if (re[(index - 2) / 2]->barR[0] < 0.6) {
            return 8;
        }
        else {
            return 9;
        }
    }
}

int jefe::move() {
    if (estado < 0) {
        if (me == 0)
            fprintf(fpLogExperts, "%i\t%i\n", t, expertType(experto));
        return rand() % A[me];
    }

    if (cycled) {
        int oldExperto = experto;
        
        // when selecting an RE expert, need to reset it
        
        resetCycle();
        experto = learner->select(satisficingExperts);
        
        if (((experto > 1) || (sencillo && (experto == 1))) && (experto != oldExperto)) {
            int ind;
            if (sencillo)
                ind = experto - 1;
            else
                ind = experto - 2;
                
            if (ind >= REcount)
                ind -= REcount;
            
            re[ind]->reset(previousActs);
        }
        
        cycled = false;
    }
    
    int a;
    if (sencillo) {
        if (experto == 0)
            a = generateAction(me, mnmx[me]->ms);
        else
            a = re[experto-1]->act(me);
    }
    else {
        
        if (experto == 0)
            a = generateAction(me, mnmx[me]->ms);
        else if (experto == 1)
            a = br->moveGreedy();
        else if (experto < (2+REcount)) // followers
            a = re[experto - 2]->act(me);
        else {
            a = generateAction(me, re[experto - 2 - REcount]->asTheFollower->follower[estado]);
        }
    }
    
    if (me == 0)
        fprintf(fpLogExperts, "%i\t%i\n", t, expertType(experto));
    
    return a;
}

void jefe::update(int *acts) {
    previousActs[0] = acts[0];
    previousActs[1] = acts[1];
    
    estado = encodeJointAction(acts);
    
    if (!sencillo)
        br->update(acts);
    
    if (experto < 0) {
        //printf("  reset cycle\n");
        cycled = true;
        t++;
        return;
    }
    
    if (experto > 1) {
        int ind = (experto - 2) / 2;
        //printf("ind = %i out of %i\n", ind, REcount);
        re[ind]->update(me, acts);
    }
    
    if (cycleFull) {
        //printf("in full cycle\n");
        if (cycleFull == 1) {
            if (tau == ((numStates)-1))
                cycled = true;
        }
        else {  // EEE
            if (tau == ((eee *)learner)->N_e[((eee *)learner)->experto])
                cycled = true;
        }
    }
    else {
        //printf("not in full cycle\n");
        //exit(1);
        if (beenThere[estado])
            cycled = true;
    }
    
    beenThere[estado] = true;
	
	t++;
	tau ++;
    
	R += M[me][acts[0]][acts[1]];
	
	if (cycled)
		learner->update(R / tau, tau);
}

void jefe::resetCycle() {
	tau = 0;
	R = 0.0;
	for (int i = 0; i < numStates; i++)
		beenThere[i] = false;
	
	if (estado >= 0)
		beenThere[estado] = true;
}

double jefe::pay(int me, int sol) {
    int a0, a1;
    
    a0 = sol / A[1];
    a1 = sol % A[1];
    
    //printf("a0 = %i\n", a0);
    //printf("a1 = %i\n", a1);
    
    return M[me][a0][a1];
}

minimaxLog *jefe::computeMaximin(int index) {
/*    printf("reading maximin %i\n", index); fflush(stdout);
    
    char fname[1024];
    sprintf(fname, "../results/maximinVals/%s", game);
    FILE *fp = fopen(fname, "r");
    
    int i;
    minimaxLog *mm = new minimaxLog(A[index]);
    if (index == 1) {
        double dummy;
        fscanf(fp, "%lf", &dummy);
        for (i = 0; i < A[0]; i++)
            fscanf(fp, "%lf", &dummy);
    }
    fscanf(fp, "%lf", &(mm->mv));
    for (i = 0; i < A[index]; i++)
        fscanf(fp, "%lf", &(mm->ms[i]));
    
    fclose(fp);
    
    return mm;
*/    
    int i, j;
    
    //numStates = A[0] * A[1];
    
    double *payoff = new double[numStates];
    int count = 0;
    for (i = 0; i < A[0]; i++) {
        for (j = 0; j < A[1]; j++) {
            payoff[count] = M[index][i][j];
            count ++;
        }
    }
    minimaxLog *mm = new minimaxLog(A[index]);
    mm->getMinimax(A, index, payoff);
    
    delete[] payoff;
    
    return mm;
}

minimaxLog *jefe::computeAttack(int index) {
/*    printf("reading attack %i\n", index); fflush(stdout);
    
    char fname[1024];
    sprintf(fname, "../results/maximinVals/%s", game);
    FILE *fp = fopen(fname, "r");
    
    int i, j;
    minimaxLog *mm = new minimaxLog(A[index]);
    for (j = 0; j < 2; j++) {
        double dummy;
        fscanf(fp, "%lf", &dummy);
        for (i = 0; i < A[0]; i++)
            fscanf(fp, "%lf", &dummy);
    }    
    if (index == 1) {
        double dummy;
        fscanf(fp, "%lf", &dummy);
        for (i = 0; i < A[0]; i++)
            fscanf(fp, "%lf", &dummy);
    }
    fscanf(fp, "%lf", &(mm->mv));
    for (i = 0; i < A[index]; i++)
        fscanf(fp, "%lf", &(mm->ms[i]));
    
    fclose(fp);
    
    return mm;
*/    
    int i, j;
    
    //numStates = A[0] * A[1];
    
    double *payoff = new double[numStates];
    int count = 0;
    for (i = 0; i < A[0]; i++) {
        for (j = 0; j < A[1]; j++) {
            payoff[count] = -M[1-index][i][j];
            count ++;
        }
    }
    minimaxLog *mm = new minimaxLog(A[index]);
    mm->getMinimax(A, index, payoff);
    
    delete[] payoff;
    
    return mm;
}

int jefe::generateAction(int index, double *pt) {
	int num = rand() % RAND_MAX;
	int i;
    
	double sum = 0.0;
	for (i = 0; i < A[index]; i++) {
		sum += pt[i] * RAND_MAX;
		if (num <= sum) {
			return i;
		}
	}
    
	printf("REexpert %i: never selected an action: num = %i\n", me, num);
    printf("%lf, %lf\n", pt[0], pt[1]);
	exit(1);
    
	return -1;
}

int jefe::encodeJointAction(int _actions[NUM_PLAYERS]) {
	return A[1] * _actions[0] + _actions[1];
}

