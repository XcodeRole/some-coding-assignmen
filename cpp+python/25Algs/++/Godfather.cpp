#include "Godfather.h"

Godfather::Godfather() {
    printf("incomplete Godfather constructor\n");
    exit(1);
}

Godfather::Godfather(int _me, int _A[2], double ***_M, char _game[1024]) {
    me = _me;
    M = _M;
    A[0] = _A[0];
    A[1] = _A[1];
    strcpy(game, _game);
    
    t = 0;
    
    int i;
	numStates = A[0];
	for (i = 1; i < 2; i++)
		numStates *= A[i];    

    createExpert();   
}

Godfather::~Godfather() {
    delete mnmx[0];
    delete mnmx[1];
    delete attack0;
    delete attack1;
    
    if (re != NULL)
        delete re;
}

void Godfather::createExpert() {
    mnmx[0] = computeMaximin(0);
    mnmx[1] = computeMaximin(1);
    attack0 = computeAttack(0);
    attack1 = computeAttack(1);

    int i, j;
    int numStates = A[0] * A[1];    
    double prodAdvantages = 0, one, two;
    int oneHigh = -1, twoHigh = -1;

    int list[100][2];
    int count = 0;
    for (i = 0; i < numStates; i++) {
        for (j = i; j < numStates; j++) {
            one = ((pay(0, i) + pay(0, j)) / 2.0) - mnmx[0]->mv;
            two = ((pay(1, i) + pay(1, j)) / 2.0) - mnmx[1]->mv;
            
            //printf("(%i, %i): one = %.3lf; two = %.3lf\n", i, j, one, two);

            if ((one > 0) && (two > 0) && ((one * two) > prodAdvantages)) {
                list[0][0] = i;
                list[0][1] = j;
                count = 1;                
                prodAdvantages = one * two;
            }
            if ((one > 0) && (two > 0) && ((one * two) == prodAdvantages)) {
                if (me == 0)
                    expectedPayout = one;
                else
                    expectedPayout = two;
                
                list[count][0] = i;
                list[count][1] = j;
                count ++;
                if (count == 100) {
                    printf("exceeded list bounds\n");
                    exit(1);
                }
            }
        }
    }
    

    //printf("maximin vals; (%.3lf; %.3lf)\n", mnmx[0]->mv, mnmx[1]->mv);
    
    if (count > 0) {
        int num = rand() % count;
        printf("%i: oneHigh = %i; twoHigh = %i\n", me, list[num][0], list[num][1]);
        re = new REexpert(me, M, A, list[num][0], list[num][1], attack0, attack1);
    }
    else
        re = NULL;
}

double Godfather::pay(int me, int sol) {
    int a0, a1;
    
    a0 = sol / A[1];
    a1 = sol % A[1];
    
    //printf("a0 = %i\n", a0);
    //printf("a1 = %i\n", a1);
    
    return M[me][a0][a1];
}


int Godfather::move() {
    if (re == NULL)
        return generateAction(mnmx[me]->ms);
    
    return re->act(me);
}

int Godfather::generateAction(double *pt) {
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


void Godfather::update(int *acts) {
    if (re != NULL)
        re->update(me, acts);
}

minimaxLog *Godfather::computeMaximin(int index) {
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

minimaxLog *Godfather::computeAttack(int index) {
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