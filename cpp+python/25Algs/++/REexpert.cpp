#include "REexpert.h"

REexpert::REexpert() {
    printf("empty constructor\n");
    exit(1);
}

REexpert::REexpert(int _me, double ***_M, int _A[2], int _a1, int _a2) {
    sencillo = true;
    enforceable = false;
    
    M = _M;
    A[0] = _A[0];
    A[1] = _A[1];
    me = _me;
    
    acts[me][0] = _a1;
    acts[me][1] = _a2;
    
    //printf("-"); fflush(stdout);
    
    barR[me] = (maxVal(acts[me][0]) + maxVal(acts[me][1])) / 2.0;
    enforceable = true;
    
    printf("(%i) Expert: %i-%i: %lf\n", me, acts[me][0], acts[me][1], barR[me]); fflush(stdout);
    
    reset();
    
    //printf("?\n"); fflush(stdout);
}

double REexpert::maxVal(int a) {
    int i;
    double max = -99999;
    
    if (me == 0) {
        //printf("<%i>", a);
        for (i = 0; i < A[1-me]; i++) {
        //    printf("%.2lf ", M[0][a][i]);
            if (M[0][a][i] > max)
                max = M[0][a][i];
        }
        //printf("\n");
    }
    else {
        //printf("<%i>", a);
        for (i = 0; i < A[1-me]; i++) {
        //    printf("%.2lf ", M[1][i][a]);
            if (M[1][i][a] > max)
                max = M[1][i][a];
        }
        //printf("\n");
    }

    return max;
}

REexpert::REexpert(int _me, double ***_M, int _A[2], int _s1, int _s2, minimaxLog *_attack0, minimaxLog *_attack1) {
    sencillo = false;
    
    M = _M;
    A[0] = _A[0];
    A[1] = _A[1];
    me = _me;
    omega = 1;
    s1 = _s1;
    s2 = _s2;
    
    attack[0] = _attack0;
    attack[1] = _attack1;
    
    acts[0][0] = s1 / A[1];
    acts[0][1] = s2 / A[1];
    
    acts[1][0] = s1 % A[1];
    acts[1][1] = s2 % A[1];

    barR[0] = (M[0][acts[0][0]][acts[1][0]] + M[0][acts[0][1]][acts[1][1]]) / 2.0;
    barR[1] = (M[1][acts[0][0]][acts[1][0]] + M[1][acts[0][1]][acts[1][1]]) / 2.0;
    
    penalty = 0.1;
    
    numStates = A[0] * A[1];
    
    assessment = new int*[numStates];
    int i;
    for (i = 0; i < numStates; i++)
        assessment[i] = new int[A[me]];

    makeAssessment(me);
    asTheTeacher = new strategyPair(me, M, A, acts, s1, s2, assessment);

    expectedValue = checkBR();
    //printf("barR = %lf; expectedValue = %lf\n", barR[me], expectedValue);
    
    enforceable = false;
    if (((expectedValue + 0.04) > barR[me]) && (barR[1-me] > 0)) {
        enforceable = true;
        //printf("enforceable\n\n");
    }
    //enforceable = true;
    //else
        //printf("not enforceable\n\n");

    makeAssessment(1-me);
    asTheFollower = new strategyPair(1-me, M, A, acts, s1, s2, assessment);    
    
    reset(); 
}

void REexpert::reset() {
    lastAction = rand() % 2;
    guilt = 0;
    guilty = false;
}

void REexpert::reset(int acciones[2]) {
    lastAction = rand() % 2;
    guilt = 0;
    guilty = false;
    
    update(me, acciones);
    
    //printf("after reset: %lf\n", guilt);
}

REexpert::~REexpert() {
    if (!sencillo) {
        int i;
        for (i = 0; i < numStates; i++) {
            delete assessment[i];
            //delete opi[i];
        }
        delete assessment;
    }
}

int REexpert::act(int index) {
    if (sencillo && (index != me)) {
        printf("wrong!\n");
        exit(1);
    }
    //printf("(%i; %i) act guilt = %lf\n", index, me, guilt);
    
    if (guilty) {
        //if (index == 0)
        //    printf("guilty: %lf\n", guilt);
        //printf("%i: submitting the attack strategy\n", me);
        return generateAction(index, attack[index]->ms);
    }
    
    //if (index == 0)
    //    printf("not guilty: %lf\n", guilt);

//    lastAction = 1 - lastAction;
    
    //printf("selecting action %i\n", acts[me][lastAction]);
    //fflush(stdout);
    
    //printf("lastAction = %i; %i\n", lastAction, acts[index][lastAction]);
    
    return acts[index][lastAction];
}

int REexpert::generateAction(int index, double *pt) {
	int num = rand() % RAND_MAX;
	int i;
    
	double sum = 0.0;
	for (i = 0; i < A[index]; i++) {
		sum += pt[i] * RAND_MAX;
		if (num <= sum) {
			return i;
		}
	}
    
    return A[index]-1;

	printf("REexpert %i: never selected an action: num = %i\n", me, num);
    for (i = 0; i < A[index]; i++)
        printf("%lf ", pt[i]);
    printf("\nsum = %lf", sum);
	exit(1);
    
	return -1;
}

void REexpert::update(int index, int acciones[2]) {
    if (sencillo) {
        lastAction = 1-lastAction;
    
        return;
    }


    if (guilty) {
        guilt += M[1-index][acciones[0]][acciones[1]] - barR[1-index];
        if (guilt < 0) {
            guilty = false;
            guilt = 0.0;
            if ((acciones[0] == acts[0][0]) && (acciones[1] == acts[1][0]))
                lastAction = 1;
            else if ((acciones[0] == acts[0][1]) && (acciones[1] == acts[1][1]))
                lastAction = 0;
        }
    }
    else {
        if (acciones[1-index] != acts[1-index][lastAction]) {
            //printf("-%.2lf-", barR[1-index]);
            guilt = (M[1-index][acciones[0]][acciones[1]] - barR[1-index]) + penalty;
            //if (index == 0)
            //    printf("(%i; %i)guilt = %lf (%.3lf instead of %.3lf)\n", index, me, guilt, M[1-index][acciones[0]][acciones[1]], barR[1-index]);
            if (guilt <= 0)
                guilt = 0.0;
            else
                guilty = true;
        }
        else
            lastAction = 1 - lastAction;
    }
    
    //if (guilty)
    //    printf("guilty: %lf\n", guilt);
    //else
    //    printf("not guilty\n");
}

void REexpert::makeAssessment(int index) {
    //printf("s1 = %i; s2 = %i\n", s1, s2);
    //printf("%i: actions: %i, %i\n", index, acts[index][0], acts[index][1]);
    int i, j;    
    for (i = 0; i < numStates; i++) {
        for (j = 0; j < A[index]; j++) {
            assessment[i][j] = 0;
            
            if (((i == s1) && (j == acts[index][1])) || ((i == s2) && (j == acts[index][0])))
                assessment[i][j] += 20;
            else if ((i != s1) && (i != s2))
                assessment[i][j] += attack[index]->ms[j] * 20;
        }
    }
    
	int hist[MAXLENGTH][NUM_PLAYERS];	
	for (i = 0; i < MAXLENGTH; i++) {
		for (j = 0; j < NUM_PLAYERS; j++) {
			hist[i][j] = 0;
		}
	}
    
    reset();
    int estado = -1, sum;
    int acciones[2];
    while (true) {
        acciones[index] = act(index);
        acciones[1-index] = rand() % A[1-index];
        update(index, acciones);
        
        if (estado >= 0) {
            //if (index == 1)
            //    printf("%i: %i, %i\n", estado, acciones[0], acciones[1]);
            
            assessment[estado][acciones[index]] ++;
            sum = 0;
            for (i = 0; i < A[index]; i++)
                sum += assessment[estado][i];
            if (sum >= 1000)
                break;
        }
        
        updateHist(hist, acciones);
        estado = state(hist);
    }
    
    // clean the priors values
    for (i = 0; i < numStates; i++) {
        sum = 0;
        for (j = 0; j < A[index]; j++)
            sum += assessment[i][j];
            
        if (sum > (20+A[index])) {
            for (j = 0; j < A[index]; j++) {
                if (((i == s1) && (j == acts[index][1])) || ((i == s2) && (j == acts[index][0])))
                    assessment[i][j] -= 20;
                else if ((i != s1) && (i != s2))
                    assessment[i][j] -= attack[index]->ms[j] * 20;
            }
        }
    }
    
    
    bool v = false;
    //if (index == me)
    //    v = true;
    if (v) {
        printf("empirical distribution:\n");
        for (i = 0; i < numStates; i++) {
            printf("%i; (%i, %i): ", i, i % (A[0]*A[1]), i / (A[0]*A[1]));
            for (j = 0; j < A[index]; j++)
                printf("%i ", assessment[i][j]);
            printf("\n");
        }
    }
}

int REexpert::state(int hist[MAXLENGTH][NUM_PLAYERS]) {
	int i, newState = 0;
	int tier = 1;
	for (i = omega-1; i >= 0; i--) {
		if (hist[i][0] == -1)
			return -1;
		
		newState += tier * encodeJointAction(hist[i]);
		tier *= A[0] * A[1];
	}
	
	return newState;	
}

int REexpert::encodeJointAction(int _actions[NUM_PLAYERS]) {
	return A[1] * _actions[0] + _actions[1];
}

void REexpert::updateHist(int hist[MAXLENGTH][NUM_PLAYERS], int acciones[2]) {
	int i, j;
	
	for (i = omega-1; i > 0; i--) {
		for (j = 0; j < NUM_PLAYERS; j++) {
			hist[i][j] = hist[i-1][j];
		}
	}
	hist[0][0] = acciones[0];
	hist[0][1] = acciones[1];
}

void REexpert::printExpert(int index, double _vu) {
    int i, a;
    printf("\t%i ", index);
    if ((index % 2) == 0) {
        printf("(teach) : ");
        for (i = 0; i < numStates; i++) {
            a = whichAction(asTheTeacher->teacher[i], me);
            if (a < 0)
                printf("m");
            else
                printf("%i", a);
        }
        printf("  vs  ");
        for (i = 0; i < numStates; i++) {
            a = whichAction(asTheTeacher->follower[i], 1-me);
            if (a < 0)
                printf("m");
            else
                printf("%i", a);
        }
        printf("    projected = %.3lf;    actual = %.3lf", barR[me], _vu);
    }
    else {
        printf("(follow): ");
        for (i = 0; i < numStates; i++) {
            a = whichAction(asTheFollower->follower[i], me);
            if (a < 0)
                printf("m");
            else
                printf("%i", a);
        }
        printf("  vs  ");
        for (i = 0; i < numStates; i++) {
            a = whichAction(asTheFollower->teacher[i], 1-me);
            if (a < 0)
                printf("m");
            else
                printf("%i", a);
        }
        printf("    projected = %.3lf;    actual = %.3lf", barR[me], _vu);
    }
    printf("\n");
}

void REexpert::printTeacherFollower() {
    int j, k;
    
    printf("\tAs the teacher (%i, %i):\n", s1, s2);
    for (j = 0; j < numStates; j++) {
        printf("\t\t%i: ", j);
        for (k = 0; k < A[me]; k++)
            printf("%.3lf ", asTheTeacher->teacher[j][k]);
        
        printf("     vs.     ");
        for (k = 0; k < A[1-me]; k++)
            printf("%.3lf ", asTheTeacher->follower[j][k]);
        printf("\n");
    }
}

void REexpert::printFollowerTeacher() {
    int j, k;
    
    printf("\n\tAs the follower (%i, %i):\n", s1, s2);
    for (j = 0; j < numStates; j++) {
        printf("\t\t%i: ", j);
        for (k = 0; k < A[me]; k++)
            printf("%.3lf ", asTheFollower->follower[j][k]);
        
        printf("     vs.     ");
        for (k = 0; k < A[1-me]; k++)
            printf("%.3lf ", asTheFollower->teacher[j][k]);
        printf("\n");
    }
}

int REexpert::whichAction(double *pi, int ind) {
    int i;
    
    for (i = 0; i < A[ind]; i++) {
        if (pi[i] > 0.99)
            return i;
    }
    
    return -1;
}

double REexpert::checkBR() {
    Rmax *br = new Rmax(1-me, A, M, 1, 0, 0.95);
    
    br->bestResponse(M, asTheTeacher->teacher);
    //br->printV();
    
    double val = 0.0;
    int s = s1;
    reset();
    int a[2], i;
    
    for (i = 0; i < 100; i++) {
        a[me] = act(me);
        a[1-me] = br->argmax(s);
        update(me, a);
        
        val += M[me][a[0]][a[1]];
        s = encodeJointAction(a);
        //printf("%i(%.3lf) ", s, guilt);
    }
    
    val /= 100.0;
    
    delete br;
    
    return val;
}


