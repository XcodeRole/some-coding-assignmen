#include "jefe++.h"

jefe_plus::jefe_plus() {
    printf("incomplete jefe_plus constructor\n");
    exit(1);
}


jefe_plus::jefe_plus(const char *nombre, int _me, int _A[2], double ***_M, double _lambda, char _game[1024]) {
    A[0] = _A[0];
    A[1] = _A[1];
    me = _me;
    M = _M;
    //lambda = _lambda;
    strcpy(game, _game);
    
    numStates = A[0] * A[1];
    t = 0;
    
    beenThere = new bool[numStates];
    
    estado = experto = -1;
    cycled = false;
    
    double aspVal;
    if (!strcmp(nombre, "S++_simp")) {
        printf("sencillo\n"); fflush(stdout);
        //numExperts = determineExperts();
        //aspVal = setAspirationHighestEnforceable_sencillo();
        //int i;
        //for (i = 0; i < REcount; i++)
        //    delete re[i];
        //delete re;
        numExperts = determineExpertsSencillo();
        sencillo = true;
    }
    else {
        numExperts = determineExperts();
        sencillo = false;
    }
    printf("numExperts = %i\n", numExperts);
    
    cycleFull = true;

    // variable lambda
    numSatExperts = 1;
    //lambda = 1 - ((1.0 / numSatExperts) * 0.04);
    lambda = 0.99;//1 - (((1.0 / numSatExperts) * 0.04)) / 2.0;
    
    if (!strcmp(nombre, "S++") || !strcmp(nombre, "S++_simp")) {
        learner = new a(_me, _A, _M, _lambda, numExperts);
        cycleFull = false;
    }
    else if (!strcmp(nombre, "exp3w++") || !strcmp(nombre, "exp3"))
        learner = new Exp3(me, (int)_lambda, 0.99, numExperts);
    else if (!strcmp(nombre, "eeew++"))
        learner = new eee(_me, _lambda, numExperts);
    else if (!strcmp(nombre, "ucbw++"))
        learner = new ucb(_me, _lambda, numExperts);
    else {
        printf("expert learner not found\n");
        exit(1);
    }
        
    int i, j;
    
/*    learner->aspiration = -99999;
	for (i = 0; i < _A[0]; i++) {
		for (j = 0; j < _A[1]; j++) {
			if (_M[me][i][j] > learner->aspiration)
				learner->aspiration = _M[me][i][j];
		}
	}
*/
    
    im = new iModel(me, A, 1);

    //setAspirationHigh();
    //setAspirationFolkEgal();
    //if (sencillo)
    //    learner->aspiration = aspVal;
    //else
    setAspirationHighestEnforceable();
    
	mu = 0.0;

    vu = new double[numExperts];
    usage = new int[numExperts];
    for (i = 0; i < numExperts; i++) {
        vu[i] = 1.0;
        usage[i] = 0.0;
    }

//    re[0]->printTeacherFollower();
//    printf("\n");
//    re[0]->printFollowerTeacher();
//    printf("\n");
    
    alwaysMM = false;
    permissibleLoss = 100.0;
    
    
    lowAspiration = 1.0;
    
    if (me == 0) {
        char nmbr[1024];
        sprintf(nmbr, "../experts/log_%s.txt", game);
        fpLogExperts = fopen(nmbr, "w");
    }
}


int jefe_plus::determineExpertsSencillo() {
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
int jefe_plus::determineExperts() {
    resetCycle();

    // first I need to generate my teaching-follower pairs (REexperts)
    determineStrategyPairs();
    int numEs = REcount * 2 + 2;
    
    br = new Rmax(me, A, M, 1, 0, 0.95);
    
    int i;
    
    satisficingExperts = new bool[numEs];
    for (i = 0; i < numEs; i++)
        satisficingExperts[i] = true;
    
    return numEs;
}

void jefe_plus::determineStrategyPairs() {
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
    
    printf("maximins: %.3lf, %.3lf\n", mnmx[0]->mv, mnmx[1]->mv);
    
    REcount = 0;
    re = new REexpert*[numSolutionPairs];
    
    for (i = 0; i < numSolutionPairs; i++) {
        if ((Theta[i].one >= mnmx[0]->mv) && (Theta[i].one > 0) && (Theta[i].two >= mnmx[1]->mv) && (Theta[i].two > 0) ) {
            //printf("creating: %lf, %lf\n", Theta[i].one, Theta[i].two);
            re[REcount] = new REexpert(me, M, A, Theta[i].s1, Theta[i].s2, attack0, attack1);

            REcount ++;
        }
    }
    
    delete[] Theta;
}

void jefe_plus::createSolutionPairs(SolutionPair *Theta) {
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


jefe_plus::~jefe_plus() {
    //printf("total loss = %lf; %i\n", ((mnmx[me]->mv * t) - mu), (int)alwaysMM);
    
    delete beenThere;
    delete learner;
    delete satisficingExperts;
    delete mnmx[0];
    delete mnmx[1];
    if (!sencillo)
        delete br;
    delete im;
    delete vu;
    delete usage;
    delete attack0;
    delete attack1;
    
    int i;
    for (i = 0; i < REcount; i++)
        delete re[i];
    delete re;
    
    if (me == 0)
        fclose(fpLogExperts);
}

int jefe_plus::expertType(int index) {
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

int jefe_plus::move() {
    //printf("m"); fflush(stdout);
    if (estado < 0) {
        if (me == 0)
            fprintf(fpLogExperts, "%i\t%i\n", t, expertType(experto));
        return rand() % A[me];
    }
    
    //printf("%i moving ... ", me); fflush(stdout);

    if (cycled) {
        int oldExperto = experto;
        // when selecting an RE expert, need to reset it
        
        resetCycle();
        
        if (sencillo)
            determineSatisficingExpertsSencillo();
        else
            determineSatisficingExperts();
        
        int i;
        numSatExperts = 0;
        for (i = 0; i < numExperts; i++) {
            if (satisficingExperts[i])
                numSatExperts ++;
        }
        //printf("<%i>", numSatExperts);
        
        experto = learner->select(satisficingExperts);
        
        //printf("%i: experto = %i\n", me, experto);
        
        if (((experto > 1) || (sencillo && (experto == 1))) && (experto != oldExperto)) {
            int ind;
            if (sencillo)
                ind = experto - 1;
            else
                ind = experto - 2;
            if (ind >= REcount)
                ind -= REcount;
            
            //printf("reset ... ");
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
        //else if ((experto < (2+REcount)) && ((experto % 2) == 0)) {
        else if ((experto % 2) == 0) {
            //printf("%i: being a teacher -- %i converts to %i\n", me, experto, (experto-2)/2);
            a = re[(experto - 2)/2]->act(me);
        }
        else {
            //printf("%i: being a follower -- %i converts to %i\n", me, experto, (experto-2)/2);
            a = generateAction(me, re[(experto - 2)/2]->asTheFollower->follower[estado]);
        }
    }
    //printf("(%i)", experto);
    //printStrat(experto);
    
    if (me == 0)
        fprintf(fpLogExperts, "%i\t%i\n", t, expertType(experto));
    
    return a;
}

void jefe_plus::update(int *acts) {
    //printf("u"); fflush(stdout);

    previousActs[0] = acts[0];
    previousActs[1] = acts[1];

	R += M[me][acts[0]][acts[1]];
	mu += M[me][acts[0]][acts[1]];

    if (!sencillo) {
        br->update(acts);
        if (estado >= 0) {
            im->update(acts, estado, t);
        }
    }

    estado = encodeJointAction(acts);
    
    //printf("p<%i>", experto); fflush(stdout);
    
    if (experto < 0) {
        cycled = true;
        t++;
        
        //printf("-"); fflush(stdout);
        return;
    }
    
    //printf("d"); fflush(stdout);

    if (experto > 1) {
        int ind;
        if (sencillo)
            ind = experto-1;
        else
            ind = (experto - 2) / 2;
        re[ind]->update(me, acts);
    }

    //printf("a"); fflush(stdout);
    
    usage[experto]++;
    double betita = 1.0 / usage[experto];
    if (betita <= (2.0 * (1.0 - lambda)))
        betita = 2.0 * (1.0 - lambda);
    
    //printf("beta = %lf; oldvu = %lf; ", betita, vu[experto]);
    vu[experto] = betita * M[me][acts[0]][acts[1]] + (1.0 - betita) * vu[experto];
    //printf("newvu = %lf\n", vu[experto]);
    
    if (cycleFull) {
        if (tau == (numStates-1))
            cycled = true;
    }
    else {
        if (beenThere[estado])
            cycled = true;
    }
    
    beenThere[estado] = true;
	
	t++;
	tau ++;
    
	if (cycled) {
        // variable lambda
        //learner->lambda = lambda = 1 - ((1.0 / numSatExperts) * 0.04);
        learner->lambda = lambda = 0.99;//1 - (((1.0 / numSatExperts) * 0.04)) / 2.0;
		learner->update(R / tau, tau);
		if (((mnmx[me]->mv * t) - mu) > permissibleLoss) {
            printf("%i: I'm losing!!!!! mu = %lf; mv*t = %lf; permissibleLoss = %lf\n", me, mu, mnmx[me]->mv * t, permissibleLoss);
			alwaysMM = true;
        }
    } 
    
    if (learner->aspiration < lowAspiration)
        lowAspiration = learner->aspiration;
    
    //printf("%i: asp=%.2lf; L=%.3lf; numSat=%i\n", me, learner->aspiration, learner->lambda, numSatExperts);
/*    if (me == 0)
        printf("\taspirations = (%.2lf<%.3lf>, ", learner->aspiration, learner->lambda);
    else
        printf("%.2lf<%.3lf>)\n", learner->aspiration, learner->lambda);*/
}

bool jefe_plus::override() {
    if (alwaysMM) {
        //printf("%i: MM override!!!!!!!!!!\n", me);
        satisficingExperts[0] = true;
        return true;
    }
    
    double brVal = br->maxV(estado) * (1.0 - 0.95);
    //if ((t > 0) && (vu[1] > brVal))
    //    brVal = vu[1];    

    int i;
    //printf("brVal = %.3lf; aspiration = %.3lf\n", brVal, learner->aspiration);
    //for (i = 0; i < numExperts; i++)
    //    printf("%.3lf  ", vu[i]);
    
    
    if (brVal >= learner->aspiration) {
        double highVal = -99999;
        int ind;
        
        for (i = 2; i < numExperts; i+=2) {
            ind = (i - 2) / 2;
            if (re[ind]->barR[me] >= learner->aspiration) {
                if (re[ind]->enforceable) {
                    if (highVal < vu[i])
                        highVal = vu[i];
                }
                
                if (im->match(re[ind]->asTheFollower->teacher)) {
                    if (highVal < vu[i+1])
                        highVal = vu[i+1];
                }
                
            }
        }
 
        //printf("high = %.4lf\n", highVal);
    
        if (vu[1] >= highVal) {
            satisficingExperts[1] = true;
            //printf("%i: BR override!!!!!!!!!!: %lf < %lf\n", me, learner->aspiration, brVal);
            //br->printV();
            return true;
        }
    }

    return false;
}

void jefe_plus::determineSatisficingExpertsSencillo() {
    int i, ind;
    for (i = 0; i < numExperts; i++)
        satisficingExperts[i] = false;
    
    bool verbose = false;
    //if (me == 0)
    //    verbose = true;
    
    if (verbose) {
        printf("  Satisficing Experts for %i (alpha = %.3lf; ", me, learner->aspiration);
        im->print();
        printf("):\n");
    }
    
    //if (override())
    //    return;
    if (alwaysMM) {
        //printf("%i: MM override!!!!!!!!!!\n", me);
        satisficingExperts[0] = true;
        return;
    }

    
    int c = 0;
    if (mnmx[me]->mv >= learner->aspiration) {
        satisficingExperts[0] = true;
        c ++;
        
        if (verbose)
            printf("\t0: minimax\n");
    }

        
    for (i = 1; i < numExperts; i++) {
        ind = i-1;
        if (re[ind]->barR[me] >= learner->aspiration) {
            if (re[ind]->enforceable) {
                satisficingExperts[i] = true; 
                c ++;
            
                if (verbose)
                    re[ind]->printExpert(i, vu[i]);
            }
        }
    }
    
    //printf("-%i-", c);
    
/*    printf("<(%.3lf)", learner->aspiration);
    for (i = 0; i < numExperts; i++)
        printf("%i", (int)(satisficingExperts[i]));
    printf(">");*/
}

void jefe_plus::determineSatisficingExperts() {
    int i, ind;
    for (i = 0; i < numExperts; i++)
        satisficingExperts[i] = false;
    
    bool verbose = false;
    //if (me == 0)
    //    verbose = true;
    
    if (verbose) {
        printf("  Satisficing Experts for %i (alpha = %.3lf; ", me, learner->aspiration);
        im->print();
        printf("):\n");
    }
    
    if (override())
        return;
    
    int c = 0;
    if (mnmx[me]->mv >= learner->aspiration) {
        satisficingExperts[0] = true;
        c ++;
        
        if (verbose)
            printf("\t0: minimax\n");
    }

        
    for (i = 2; i < numExperts; i+=2) {
        ind = (i - 2) / 2;
        if (re[ind]->barR[me] >= learner->aspiration) {
            if (re[ind]->enforceable) {
                satisficingExperts[i] = true; 
                c ++;
            
                if (verbose)
                    re[ind]->printExpert(i, vu[i]);
            }
            
            if (im->match(re[ind]->asTheFollower->teacher)) {  // see if we should add the follower
                satisficingExperts[i+1] = true;
                c++;
                
                if (verbose)
                    re[ind]->printExpert(i+1, vu[i+1]);
            }
        }
    }
    
    double brVal = br->maxV(estado) * (1.0 - 0.95);
    //if ((t > 0) && (vu[1] > brVal))
    //    brVal = vu[1];    
    
    //if (verbose)
    //    printf("%i: best response yields: %.3lf\n", me, br->maxV(estado));
    
    if ((c == 0) || (brVal >= learner->aspiration)) {
        satisficingExperts[1] = true;
        c ++;
        
        if (verbose) {
            printf("\t1 (BR)    : ");
            for (i = 0; i < numStates; i++) {
                printf("%i", br->argmax(i));
            }
            printf("              projected = %.3lf;    actual = %.3lf\n", brVal, vu[1]);
        }
    }
    
    //printf("-%i-", c);
    
/*    printf("<(%.3lf)", learner->aspiration);
    for (i = 0; i < numExperts; i++)
        printf("%i", (int)(satisficingExperts[i]));
    printf(">");*/
}

void jefe_plus::resetCycle() {
	tau = 0;
	R = 0.0;
	for (int i = 0; i < numStates; i++)
		beenThere[i] = false;
	
	if (estado >= 0)
		beenThere[estado] = true;
}

double jefe_plus::pay(int me, int sol) {
    int a0, a1;
    
    a0 = sol / A[1];
    a1 = sol % A[1];
    
    //printf("a0 = %i\n", a0);
    //printf("a1 = %i\n", a1);
    
    return M[me][a0][a1];
}

minimaxLog *jefe_plus::computeMaximin(int index) {
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

minimaxLog *jefe_plus::computeAttack(int index) {
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

void jefe_plus::setAspirationHigh() {
    if (REcount == 0) {
        learner->aspiration = mnmx[me]->mv;
        printf("no good expert\n");
        return;
    }
    
	int i, j, index = -1;
	double high = mnmx[me]->mv;
	int s;
    
	for (i = 0; i < REcount; i++) {
        if (re[i]->barR[me] > high) {
            high = re[i]->barR[me];
            index = i;
        }
	}

	learner->aspiration = high;
	printf("%i: initial aspiration level = %.3lf\n", me, learner->aspiration);
}

void jefe_plus::setAspirationFolkEgal() {
    if (REcount == 0) {
        learner->aspiration = mnmx[me]->mv;
        printf("no good expert\n");
        return;
    }

	int i, j, index = -1;
	double high = 0.0/*mnmx[me]->mv*/, theMin;
	int s;
    
	for (i = 0; i < REcount; i++) {
        theMin = re[i]->barR[me];
        if (theMin > re[i]->barR[1-me])
            theMin = re[i]->barR[1-me];
        
        //printf("theMin = %lf\n", theMin);
        
        if (theMin > high) {
            high = theMin;
            index = i;
        }
	}
    
    //printf("index = %i\n", index);
    //fflush(stdout);
    
	learner->aspiration = re[index]->barR[me];
    if (learner->aspiration < mnmx[me]->mv)
        learner->aspiration = mnmx[me]->mv;
	printf("%i: initial aspiration level = %.3lf\n", me, learner->aspiration);
}

double jefe_plus::setAspirationFolkEgal_sencillo() {
    if (REcount == 0) {
        return mnmx[me]->mv;
    }

	int i, j, index = -1;
	double high = 0.0/*mnmx[me]->mv*/, theMin;
	int s;
    
	for (i = 0; i < REcount; i++) {
        theMin = re[i]->barR[me];
        if (theMin > re[i]->barR[1-me])
            theMin = re[i]->barR[1-me];
        
        if (theMin > high) {
            high = theMin;
            index = i;
        }
	}
    
	double val = re[index]->barR[me];
    if (val < mnmx[me]->mv)
        val = mnmx[me]->mv;
	printf("%i: initial aspiration level = %.3lf\n", me, val);
    
    return val;
}

void jefe_plus::setAspirationHighestEnforceable() {
    if (REcount == 0) {
        learner->aspiration = mnmx[me]->mv;
        printf("no good expert\n");
        return;
    }    
    
	int i, j, index = -1;
	double high = 0.0/*mnmx[me]->mv*/, val;
	int s;
    
    //printf("REcount = %i\n", REcount);
    //fflush(stdout);
    
	for (i = 0; i < REcount; i++) {
        if (re[i]->enforceable && (re[i]->barR[me] > high)) {
            high = re[i]->barR[me];
            index = i;
        }
	}
    
    //printf("index = %i\n", index);
    //fflush(stdout);
    if (index == -1) {
        printf("nothing is enforceable\n"); fflush(stdout);
        setAspirationFolkEgal();
    }
    else {
        learner->aspiration = re[index]->barR[me];
        if (learner->aspiration < mnmx[me]->mv)
            learner->aspiration = mnmx[me]->mv;
        printf("%i: initial aspiration level = %.3lf\n", me, learner->aspiration);
    }
}

double jefe_plus::setAspirationHighestEnforceable_sencillo() {
    if (REcount == 0) {
        return mnmx[me]->mv;
    }    
    
	int i, j, index = -1;
	double high = 0.0/*mnmx[me]->mv*/, val;
	int s;
    
    //printf("REcount = %i\n", REcount);
    //fflush(stdout);
    
	for (i = 0; i < REcount; i++) {
        if (re[i]->enforceable && (re[i]->barR[me] > high)) {
            high = re[i]->barR[me];
            index = i;
        }
	}
    
    //printf("index = %i\n", index);
    //fflush(stdout);
    if (index == -1) {
        printf("nothing is enforceable\n"); fflush(stdout);
        return setAspirationFolkEgal_sencillo();
    }
    else {
        double val = re[index]->barR[me];
        if (val < mnmx[me]->mv)
            val = mnmx[me]->mv;
        printf("%i: initial aspiration level = %.3lf\n", me, val);
        
        return val;
    }
}


void jefe_plus::printStrat(int strat) {
    printf("<");
    int i, j;
    if (strat == 0) {
        for (i = 0; i < numStates; i++)
            printf("x");
    }
    else if (strat == 1) {
        for (i = 0; i < numStates; i++)
            printf("%i", br->argmax(i));
    }
    else {
        int ind = (strat-2) / 2, a;
        double **agente = re[ind]->asTheTeacher->teacher;
        if (strat % 2)
            agente = re[ind]->asTheFollower->follower;
        double **agent;
        for (i = 0; i < numStates; i++) {
            a = -1;
            for (j = 0; j < A[me]; j++) {
                if (agente[i][j] > 0.99)
                    a = j;
            }
            if (a < 0)
                printf("m");
            else
                printf("%i", a);
        }
    }
    
    printf("> ");
}

int jefe_plus::generateAction(int index, double *pt) {
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

int jefe_plus::encodeJointAction(int _actions[NUM_PLAYERS]) {
	return A[1] * _actions[0] + _actions[1];
}


