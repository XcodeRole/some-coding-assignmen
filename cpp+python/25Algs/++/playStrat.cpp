#include "playStrat.h"

playStrat::playStrat() {
    printf("incomplete playStrat constructor\n");
    exit(1);
}

playStrat::playStrat(int _me, int _A[2], double ***_M, char _game[1024], int _number) {
    me = _me;
    M = _M;
    A[0] = _A[0];
    A[1] = _A[1];
    strcpy(game, _game);
    expertNumber = _number;

    expertExists = true;
    
    int i;
	numStates = A[0];
	for (i = 1; i < 2; i++)
		numStates *= A[i];
    
    if (expertNumber == 0) 
        mnmx[me] = computeMaximin(me);
    else if (expertNumber == 1) {
        //printf("against the br agent\n");
        br = new Rmax(me, A, M, 1, 0, 0.95);
    }
    else {
        //printf("one of the experts\n");
        createExpert();
    }
    
    estado = rand() % (A[0] * A[1]);
    printf("initializing to state %i\n", estado);
}

playStrat::~playStrat() {
    if (expertNumber == 0)
        delete mnmx[me];
    else if (expertNumber == 1)
        delete br;
    else {
        delete mnmx[0];
        delete mnmx[1];
        delete attack0;
        delete attack1;
        if (expertExists)
            delete re;
    }
}

void playStrat::createExpert() {
    mnmx[0] = computeMaximin(0);
    mnmx[1] = computeMaximin(1);
    attack0 = computeAttack(0);
    attack1 = computeAttack(1);
    
    int i, j;
    int numStates = A[0] * A[1];    
    double prodAdvantages = 0, one, two;
    
    int count = 2;
    for (i = 0; i < numStates; i++) {
        for (j = i; j < numStates; j++) {
            one = ((pay(0, i) + pay(0, j)) / 2.0);
            two = ((pay(1, i) + pay(1, j)) / 2.0);
            
            //printf("(%i, %i): one = %.3lf; two = %.3lf\n", i, j, one, two);
            if ((one >= mnmx[0]->mv) && (one > 0) && (two >= mnmx[1]->mv) && (two > 0) ) {
                count += 2;
                if (count > expertNumber) {
                    re = new REexpert(me, M, A, i, j, attack0, attack1);
                    //printf("created expert %i\n", expertNumber);
                    
                    if (!(re->enforceable) && ((expertNumber % 2) == 0)) {
                        printf("teacher that can't enforce -- don't run\n");
                        expertExists = false;
                    }
                    
                    return;
                }
            }
        }
    }
    
    printf("expert %i does not seem to exist in this game\n", expertNumber);
    //exit(1);
    expertExists = false;
}

double playStrat::pay(int me, int sol) {
    int a0, a1;
    
    a0 = sol / A[1];
    a1 = sol % A[1];
    
    return M[me][a0][a1];
}


int playStrat::move() {
    if (expertNumber == 0)
        return generateAction(mnmx[me]->ms);
    else if (expertNumber == 1)
        return br->moveGreedy();
    else if ((expertNumber % 2) == 0)
        return re->act(me);
    else {
        return generateAction(re->asTheFollower->follower[estado]);
    }
}

int playStrat::generateAction(double *pt) {
	int num = rand() % RAND_MAX;
	int i;
    
	double sum = 0.0;
	for (i = 0; i < A[me]; i++) {
		sum += pt[i] * RAND_MAX;
		if (num <= sum) {
			return i;
		}
	}
    
    return A[me]-1;
    
	printf("REexpert %i: never selected an action: num = %i\n", me, num);
    printf("%lf, %lf\n", pt[0], pt[1]);
	exit(1);
    
	return -1;
}


void playStrat::update(int *acts) {
    if (expertNumber == 1)
        br->update(acts);
    else if ((expertNumber > 0) && ((expertNumber % 2) == 0))
        re->update(me, acts);
    
    estado = A[1] * acts[0] + acts[1];
}

minimaxLog *playStrat::computeMaximin(int index) {
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

minimaxLog *playStrat::computeAttack(int index) {
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