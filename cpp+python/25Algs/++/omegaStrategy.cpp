#include "omegaStrategy.h"

omegaStrategy::omegaStrategy() {
	int i;
	
	me = 0;
	omega = 1;
	for (i = 0; i < NUM_PLAYERS; i++) {
		A[i] = 0;
	}
	
	//init();
}
	
omegaStrategy::omegaStrategy(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega) {
	int i;
	
	me = _me;
	omega = _omega;
	for (i = 0; i < NUM_PLAYERS; i++) {
		A[i] = _A[i];
	}
	
	init(_M);
	
	r = 0.0;
	usageCount = 0;
}

void omegaStrategy::init(double ***_M) {
	int i;
	
	numJointActions = A[0];
	for (i = 1; i < NUM_PLAYERS; i++)
		numJointActions *= A[i];
	
	numStates = pow(numJointActions, omega);
	
	pi = new double*[numStates];
	for (i = 0; i < numStates; i++) {
		pi[i] = new double[A[me]];
	}
	
	initV(_M);
	
	verbose = false;
}

omegaStrategy::~omegaStrategy() {
	int i;
	for (i = 0; i < numStates; i++) {
		delete pi[i];
		delete V[i];
	}
	delete pi;
	delete V;
	delete Valor;
}

int omegaStrategy::state(int hist[MAXLENGTH][NUM_PLAYERS]) {
	int i, newState = 0;
	int tier = 1;
	for (i = omega-1; i >= 0; i--) {
		if (hist[i][0] == -1)
			return -1;
		
		newState += tier * encodeJointAction(hist[i]);
		tier *= numJointActions;
	}
	
	return newState;	
}

int omegaStrategy::encodeJointAction(int _actions[NUM_PLAYERS]) {
	//int num = A[1-me] * _actions[me] + _actions[1-me]; // assume 2 players here
	int num = A[1] * _actions[0] + _actions[1];
	//printf("%i * %i + %i = %i\n", A[1], _actions[0], _actions[1], num);
	return num;
}

void omegaStrategy::setPi(double **_pi) {
	int i, j;
	
	for (i = 0; i < numStates; i++) {
		for (j = 0; j < A[me]; j++) {
			pi[i][j] = _pi[i][j];
		}
	}
}

int omegaStrategy::selectAction(int s) {
	double num = (double)(rand()) / (double)(RAND_MAX);
	int i;
	double sum = 0.0;
	for (i = 0; i < A[me]; i++) {
		sum += pi[s][i];
		if (num < sum)
			break;
	}
	if (i == A[me])
		i--;
	
	
	return i;
}

// all of this has to do with computing a best response
void omegaStrategy::initV(double ***_M) {
   	gamma = 0.99;
    
	V = new double*[numStates];
	int i, j;
    
/*    double ave = 0.0;
    for (i = 0; i < A[0]; i++) {
        for (j = 0; j < A[1]; j++) {
            ave += _M[me][i][j];
        }
    }
    ave /= A[0] * A[1];
    printf("average payoff = %lf\n", ave);
    //printf("gamma = %lf\n", gamma);
    ave /= (1.0 - gamma);
    printf("average V = %lf\n", ave);
*/    
    //exit(1);
    
	for (i = 0; i < numStates; i++) {
		V[i] = new double[A[me]];
		for (j = 0; j < A[me]; j++)
			V[i][j] = 0.0;
	}
	
	Valor = new double[numStates];
}

void omegaStrategy::buildString(char nombre[1024], omegaStrategy *opi) {
    sprintf(nombre, "brs/shapleys_");    
    
    int s, j, a;
    for (s = 0; s < numStates; s++) {
        a = 0;
        for (j = 1; j < A[me]; j++) {
            if (pi[s][j] > pi[s][a])
                a = j;
        }

        sprintf(nombre, "%s%i", nombre, a);
    }
    sprintf(nombre, "%s_", nombre);
    for (s = 0; s < numStates; s++) {
        a = 0;
        for (j = 1; j < A[1-me]; j++) {
            if (opi->pi[s][j] > opi->pi[s][a])
                a = j;
        }
        
        sprintf(nombre, "%s%i", nombre, a);
    }
    sprintf(nombre, "%s.txt", nombre);
}

void omegaStrategy::bestResponse(double ***M, omegaStrategy *opi, int option) {
	int i = 0;
    int s, j, cer;

    bool notwritten = true;
    if (option == 2) {
        char nombre[1024];
        buildString(nombre, opi);
        FILE *fp;
        if ((fp = fopen(nombre, "r")) != NULL) {
            notwritten = false;
            for (s = 0; s < numStates; s++) {
                for (j = 0; j < A[me]; j++)
                    fscanf(fp, "%lf", &(V[s][j]));
            }
            fclose(fp);
        }        
    }
    
    if (notwritten) {
        
        
/*        double change = 99999;
        int c = 0;
        while (change > 0.1) {//0.000001) {
            printV();
            change = greedyValueIteration(M, opi);
            printf("%i: change = %lf\n", c, change);
            c++;
        }
*/        
        int c = smartGreedyValueIteration(M, opi);
        //printf("iterations to convergence: %i\n", c);
        //exit(1);
        
        if (option == 2) {
            char nombre[1024];
            buildString(nombre, opi);
            FILE *fp = fopen(nombre, "w");
            for (s = 0; s < numStates; s++) {
                for (j = 0; j < A[me]; j++)
                    fprintf(fp, "%lf ", V[s][j]);
                fprintf(fp, "\n");
            }
            fclose(fp);
        }
    }
	
	if (verbose)
		printf("        %i BR: ", me);
	for (s = 0; s < numStates; s++) {
		Valor[s] = V[s][argmax(s)] * (1.0 - gamma);
        
        cer = 0;
        for (j = 0; j < A[me]; j++) {
            if (V[s][j] == V[s][argmax(s)]) {
                pi[s][j] = 1.0;
                cer ++;
            }
            else
                pi[s][j] = 0.0;
        }
        for (j = 0; j < A[me]; j++) {
            pi[s][j] /= cer;
        }
		
		if (verbose)
			printf("%i    ", argmax(s));
	}
	if (verbose) {
		printf("\n     %i Valor: ", me);
		for (s = 0; s < numStates; s++)
			printf("%.2lf ", Valor[s]);
		printf("\n");
	}
}

int omegaStrategy::getAction(double *pi_er, int numActs) {
    int i;
    
    for (i = 0; i < numActs; i++) {
        if (pi_er[i] > 0.5)
            return i;
    }
    
    printf("not a valid pure strategy\n");
    exit(1);
    
    return -1;
}

double omegaStrategy::estimatePureValue(double ***M, omegaStrategy *opi, int _s) {
    int i, j;
    int its = 100;
	int hist[MAXLENGTH][NUM_PLAYERS];
	
	for (i = 0; i < MAXLENGTH; i++) {
		for (j = 0; j < NUM_PLAYERS; j++) {
			hist[i][j] = 0;
		}
	}
    
    int meAct, heAct;
    int s = _s;
    int acts[2];
    double vals[2] = {0.0, 0.0};
    for (i = 0; i < its; i++) {
        acts[me] = getAction(pi[s], A[me]);
        acts[1-me] = getAction(opi->pi[s], A[1-me]);
        updateHist(hist, acts[0], acts[1]);
        vals[0] += M[0][acts[0]][acts[1]];
        //vals[1] += M[1][acts[0]][acts[1]];        
    }
    
    Valor[_s] = (vals[0] / its) * (1.0 - gamma);
    
    return Valor[_s];
}

void omegaStrategy::valuesGivenStrategyPair(double ***M, omegaStrategy *opi) {
	double change = 99999;
    //int c = 0;
	//while (change > 0.000001) {
	//	change = valueIteration(M, opi);
    //    c ++;
	//}
    //printf("count = %i\n", c);
    int c = smartValueIteration(M, opi);
    //printf("c = %i\n", c);
	
    //printV();
    
	int s, i;
	for (s = 0; s < numStates; s++) {
		Valor[s] = 0;
		for (i = 0; i < A[me]; i++)
			Valor[s] += V[s][i] * pi[s][i] * (1.0 - gamma);
	}
	if (verbose) {
		printf("     %i Valor: ", me);
		for (s = 0; s < numStates; s++)
			printf("%.2lf ", Valor[s]);
		printf("\n\n");
	}
}

double omegaStrategy::valueIteration(double ***M, omegaStrategy *opi) {
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
			nv = 0.0;
			for (j = 0; j < A[1-me]; j++) {
				if (me == 0)
					updateHist(hist, a, j);
				else
					updateHist(hist, j, a);
				sprime = state(hist);
				if (me == 0)
					nv += opi->pi[s][j] * (M[me][a][j] + gamma * expectedV(sprime));
				else
					nv += opi->pi[s][j] * (M[me][j][a] + gamma * expectedV(sprime));
			}
			change += fabs(V[s][a] - nv);
			V[s][a] = nv;
		}
	}
	
	return change;	
}

int omegaStrategy::smartValueIteration(double ***M, omegaStrategy *opi) {
    gamma = 0.84;
    
    double totalErrorTolerance = 0.000001;
    
    double change;
    int c = 0;
    
    do {
        change = 99999;
        while (change > totalErrorTolerance) {//0.000001) {
            //printV();
            change = valueIteration(M, opi);
            //printf("%i: change = %lf\n", c, change);
            c++;
        }
        
        //printf("%i ... ", c);
        
        if (gamma < 0.99) {
            int i, j;
            for (i = 0; i < numStates; i++) {
                for (j = 0; j < A[me]; j++)
                    V[i][j] *= (1.0 - gamma);
            }
            gamma += 1.0;
            gamma /= 2.0;
            for (i = 0; i < numStates; i++) {
                for (j = 0; j < A[me]; j++)
                    V[i][j] /= (1.0 - gamma);
            }
        }
    } while (gamma < 0.99);
    
    return c;
}


int omegaStrategy::smartGreedyValueIteration(double ***M, omegaStrategy *opi) {
    gamma = 0.84;
    
    double totalErrorTolerance = 0.000001;
    
    double change;
    int c = 0;
    
    do {
        change = 99999;
        while (change > totalErrorTolerance) {//0.000001) {
            //printV();
            change = greedyValueIteration(M, opi);
            //printf("%i: change = %lf\n", c, change);
            c++;
        }
        
        //printf("%i ... ", c);
        
        if (gamma < 0.99) {
            int i, j;
            for (i = 0; i < numStates; i++) {
                for (j = 0; j < A[me]; j++)
                    V[i][j] *= (1.0 - gamma);
            }
            gamma += 1.0;
            gamma /= 2.0;
            for (i = 0; i < numStates; i++) {
                for (j = 0; j < A[me]; j++)
                    V[i][j] /= (1.0 - gamma);
            }
        }
    } while (gamma < 0.99);
    
    return c;
}

double omegaStrategy::greedyValueIteration(double ***M, omegaStrategy *opi) {
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
			if (opi->pi[s][j] == -99999)
				nv = M[me][a][j] + gamma * (1.0 / (1.0 - gamma));
			else {		
				nv = 0.0;
				for (j = 0; j < A[1-me]; j++) {				
					if (me == 0)
						updateHist(hist, a, j);
					else
						updateHist(hist, j, a);
					sprime = state(hist);
					if (me == 0)
						nv += opi->pi[s][j] * (M[me][a][j] + gamma * maxV(sprime));
					else
						nv += opi->pi[s][j] * (M[me][j][a] + gamma * maxV(sprime));
				}
			}
			change += fabs(V[s][a] - nv);
			V[s][a] = nv;
		}
	}

	return change;
}

void omegaStrategy::updateHist(int hist[MAXLENGTH][NUM_PLAYERS], int a0, int a1) {
	int i, j;
	
	for (i = omega-1; i > 0; i--) {
		for (j = 0; j < NUM_PLAYERS; j++) {
			hist[i][j] = hist[i-1][j];
		}
	}
	hist[0][0] = a0;
	hist[0][1] = a1;
}

int omegaStrategy::argmax(int s) {
	int max = 0;
	int i;
    
    //tieFlag = false;
	for (i = 1; i < A[me]; i++) {
        if (V[s][max] < V[s][i]) {
            max = i;
            //tieFlag = false;
        }
        else if (V[s][max] == V[s][i]) {
            //tieFlag = true;
        }
	}
    
	return max;
}

double omegaStrategy::maxV(int s) {
	int max = 0;
	int i;
	
	for (i = 1; i < A[me]; i++) {
		if (V[s][max] < V[s][i])
			max = i;
	}
	
	return V[s][max];	
}

double omegaStrategy::expectedV(int s) {
	double sum = 0.0;
	int i;
	
	for (i = 0; i < A[me]; i++)
		sum += pi[s][i] * V[s][i];
	
	return sum;
}


void omegaStrategy::printV() {
	int s, a;
	for (s = 0; s < numStates; s++) {
		printf("%i: ", s);
		for (a = 0; a < A[me]; a++) {
			printf("%.3lf\t", V[s][a] * (1.0 - gamma));
		}
		printf("\n");
	}
	printf("\n");
}

void omegaStrategy::printStrat() {
	int s, a;
	for (s = 0; s < numStates; s++) {
        if ((pi[s][0] < 0.00001) || (pi[s][0] > 0.99999))
            printf("%i", 1-(int)(pi[s][0]+0.1));
        else {
            printf("m");
        }
	}
}

void omegaStrategy::printStrat3() {
	int s, a;
	for (s = 0; s < numStates; s++) {
        if (pi[s][0] > 0.99999)
            printf("0");
        else if (pi[s][1] > 0.99999)
            printf("1");
        else if (pi[s][2] > 0.99999)
            printf("2");
        else
            printf("m");
	}
}

bool omegaStrategy::matches(omegaStrategy *os) {
	int s, i;
	
	for (s = 0; s < numStates; s++) {
		for (i = 0; i < A[me]-1; i++) {
			if (fabs(os->pi[s][i] - pi[s][i]) > (1.0 - (1.0 / A[me]))) {
				return false;
			}
		}
	}
	
	return true;
}

