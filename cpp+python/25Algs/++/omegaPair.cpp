#include "omegaPair.h"

#define filewriting     0

omegaPair::omegaPair() {
	printf("In omegaPair constructor that doesn't initialize everything!!!\n");
    exit(1);
}

omegaPair::omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType) {  // ATTACKER and UNKNOWN
	int i, j;
	
	me = _me;
	omega = _omega;
	M = _M;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	//opponentType = _opponentType;
	
	r = 0.0;
	usageCount = 0;
	
	bool verbose = false;
	
	if (_opponentType == ATTACKER) {
		minimaxLog *mm = new minimaxLog(A[me]);
		double *payoff = new double[A[0] * A[1]];
		int count = 0;
		for (i = 0; i < A[0]; i++) {
			for (j = 0; j < A[1]; j++) {
				payoff[count] = M[me][i][j];
				count++;
			}
		}
		
		mm->getMinimax(A, me, payoff);
		ares = new omegaStrategy(me, A, _M, omega);
		for (i = 0; i < ares->numStates; i++) {
			ares->Valor[i] = mm->mv;
			for (j = 0; j < A[me]; j++)
				ares->pi[i][j] = mm->ms[j];
		}
		
		delete mm;
		delete payoff;
		
		mm = new minimaxLog(A[1-me]);
		payoff = new double[A[0] * A[1]];
		count = 0;
		for (i = 0; i < A[0]; i++) {
			for (j = 0; j < A[1]; j++) {
				payoff[count] = M[1-me][i][j];
				count++;
			}
		}
		
		mm->getMinimax(A, 1-me, payoff);
		thars = new omegaStrategy(1-me, A, _M, omega);
		for (i = 0; i < thars->numStates; i++) {
			thars->Valor[i] = mm->mv;
			for (j = 0; j < A[1-me]; j++)
				thars->pi[i][j] = mm->ms[j];
		}
		
		if (verbose) {
			printf("Maximin\n-------------\n");
			printf("%i: %.2lf: ", me, ares->Valor[0]);
			for (i = 0; i < A[me]; i++)
				printf("%.2lf ", ares->pi[0][i]);
			printf("\n%i: %.2lf: ", 1-me, thars->Valor[0]);
			for (i = 0; i < A[1-me]; i++)
				printf("%.2lf ", thars->pi[0][i]);
			printf("\n\n");
		}
		
		delete mm;
		delete payoff;
		
		hypoares = hypothars = NULL;
	}
	else if (_opponentType == UNKNOWN) {
		thars = new omegaStrategy(1-me, A, _M, omega);
		double split = 1.0 / A[1-me];
		for (i = 0; i < thars->numStates; i++) {
			for (j = 0; j < A[1-me]; j++)
				thars->pi[i][j] = split;
		}
		ares = new omegaStrategy(me, A, _M, omega);
		ares->bestResponse(M, thars, 0);
		thars->valuesGivenStrategyPair(M, ares);
		
		hypothars = new omegaStrategy(1-me, A, _M, omega);
		hypoares = new omegaStrategy(me, A, _M, omega);
	}
}

omegaPair::omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType, int *_pi) {  // TEACHER and FOLLOWER
	int i, j;
	
	me = _me;
	omega = _omega;
	M = _M;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	//opponentType = _opponentType;
	
	if (_opponentType == TEACHER) {
		thars = new omegaStrategy(1-me, A, _M, omega);
		for (i = 0; i < thars->numStates; i++) {
			for (j = 0; j < A[1-me]; j++) {
				if (_pi[i] == j)
					thars->pi[i][j] = 1.0;
				else
					thars->pi[i][j] = 0.0;
			}
		}
		
		ares = new omegaStrategy(me, A, _M, omega);
		ares->bestResponse(M, thars, filewriting);
		thars->valuesGivenStrategyPair(M, ares);
	}
	else if (_opponentType == FOLLOWER) {
		ares = new omegaStrategy(me, A, _M, omega);
		int i, j;
		for (i = 0; i < ares->numStates; i++) {
			for (j = 0; j < A[me]; j++) {
				if (_pi[i] == j)
					ares->pi[i][j] = 1.0;
				else
					ares->pi[i][j] = 0.0;
			}
		}
		thars = new omegaStrategy(1-me, A, _M, omega);
		thars->bestResponse(M, ares, filewriting);
		ares->valuesGivenStrategyPair(M, thars);
	}
	hypoares = hypothars = NULL;
}

omegaPair::omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _opponentType, double **_pi) {  // TEACHER and FOLLOWER
	int i, j;
	
	me = _me;
	omega = _omega;
	M = _M;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
	//opponentType = _opponentType;
	
	if (_opponentType == TEACHER) {
		thars = new omegaStrategy(1-me, A, _M, omega);
		for (i = 0; i < thars->numStates; i++) {
			for (j = 0; j < A[1-me]; j++) {
				thars->pi[i][j] = _pi[i][j];
			}
		}
		
		ares = new omegaStrategy(me, A, _M, omega);
		ares->bestResponse(M, thars, filewriting);
		thars->valuesGivenStrategyPair(M, ares);
	}
	else if (_opponentType == FOLLOWER) {
		ares = new omegaStrategy(me, A, _M, omega);
		int i, j;
		for (i = 0; i < ares->numStates; i++) {
			for (j = 0; j < A[me]; j++) {
				ares->pi[i][j] = _pi[i][j];
			}
		}
		thars = new omegaStrategy(1-me, A, _M, omega);
		thars->bestResponse(M, ares, filewriting);
        //printf("\n");
        //thars->printV();
		ares->valuesGivenStrategyPair(M, thars);
	}
	hypoares = hypothars = NULL;
}

omegaPair::omegaPair(int _me, int _A[NUM_PLAYERS], double ***_M, int *hisStrat, int **myStrats, int _s) {
    int i, j, k;
    
    me = _me;
    omega = 1;
    M = _M;
	for (i = 0; i < NUM_PLAYERS; i++)
		A[i] = _A[i];
    
    thars = new omegaStrategy(1-me, A, _M, omega);
    for (i = 0; i < thars->numStates; i++) {
        for (j = 0; j < A[1-me]; j++) {
            if (hisStrat[i] == j)
                thars->pi[i][j] = 1.0;
            else
                thars->pi[i][j] = 0.0;
        }
    }
    
    ares = new omegaStrategy(me, A, _M, omega);

    // pick the strategy that does the best against thars
    double high = -99999;
    int highIndex = -1;
    for (i = 0; i < MAX_EXPERTS; i++) {
        //printf("(%i)", i);
        //fflush(stdout);
        for (j = 0; j < ares->numStates; j++) {
            for (k = 0; k < A[me]; k++) {
                if (myStrats[i][j] == k)
                    ares->pi[j][k] = 1.0;
                else
                    ares->pi[j][k] = 0.0;
            }
        }
        
        ares->estimatePureValue(M, thars, _s);
        if (ares->Valor[_s] > high) {
            high = ares->Valor[_s];
            highIndex = i;
        }
    }
    
    for (j = 0; j < ares->numStates; j++) {
        for (k = 0; k < A[me]; k++) {
            if (myStrats[highIndex][j] == k)
                ares->pi[j][k] = 1.0;
            else
                ares->pi[j][k] = 0.0;
        }
    }
    
    ares->Valor[_s] = high;
    //thars->valuesGivenStrategyPair(M, ares);
}

omegaPair::~omegaPair() {
	delete ares;
	delete thars;
	if (hypoares != NULL) {
		delete hypoares;
		delete hypothars;
	}
}

void omegaPair::updateUnknown(int **_kappa, int s) {
	int j, sum = 0;

	for (j = 0; j < A[1-me]; j++)
		sum += _kappa[s][j];
	
	for (j = 0; j < A[1-me]; j++) {
		if (sum == 0)
			thars->pi[s][j] = -99999;
		else
			thars->pi[s][j] = (double)(_kappa[s][j]) / (double)sum;
	}

	ares->bestResponse(M, thars, 0);
	thars->valuesGivenStrategyPair(M, ares);
	
	bool verbose = false;
	
	if (verbose && (me == 0)) {
		printf("kappa: ");
		for (j = 0; j < thars->numStates; j++)
			printf("%.2lf ", thars->pi[j][0]);
		printf("\n   BR: ");
		for (j = 0; j < ares->numStates; j++)
			printf("%i   ", ares->argmax(j));
		printf("\n V_me: ");
		for (j = 0; j < ares->numStates; j++)
			printf("%.2lf ", ares->Valor[j]);
		printf("\n");
		//ares->printV();
	}
}

double omegaPair::shouldHave(int startState, int sCuenta) {
	int i, j;
	double sum = 0.0;
	int s = startState;
	int hist[MAXLENGTH][NUM_PLAYERS];
	int ares_act, thars_act;
	
	//printf("\ns = %i\n", s);
	for (i = 0; i < sCuenta; i++) {
		for (j = 0; j < A[1-me]; j++) {
			if (thars->pi[s][j] > 0.999) {
				thars_act = j;
				break;
			}
		}
		for (j = 0; j < A[me]; j++) {
			if (ares->pi[s][j] > 0.999) {
				ares_act = j;
				break;
			}
		}
		
		//printf("%i: %i, %i\n", s, ares_act, thars_act);
				
		if (me == 0) {
			sum += M[me][ares_act][thars_act];
			hist[0][0] = ares_act;
			hist[0][1] = thars_act;
		}
		else {
			sum += M[me][thars_act][ares->argmax(s)];
			hist[0][1] = ares_act;
			hist[0][0] = thars_act;
		}
		
		s = ares->state(hist);
		//printf("(%i, %i) : s = %i\n", hist[0][0], hist[0][1], s);
	}
	
	return sum / sCuenta;
}

double omegaPair::average() {
	if (usageCount == 0)
		return -99;
	
	return r / usageCount;
}

double omegaPair::socialWelfare(int s) {
	return (ares->Valor[s] + thars->Valor[s]) / 2.0;
}

bool omegaPair::computeHypotheticals(int samps, int **_kappa, int estado, double aspiration, double *explore) {
	int i, j, k, val;
	int numHypos = pow(A[1-me], hypothars->numStates);
	int *digits = new int[hypothars->numStates];
	int *lacking = new int[hypothars->numStates];
	int sum;
	
	//printf("\n--------\nnumHypotheticals = %i (%i)\n", numHypos, estado);
	
	for (i = 0; i < hypothars->numStates; i++) {
		sum = -2;
		for (j = 0; j < A[1-me]; j++) {
			sum += _kappa[i][j];
		}
		
		sum = samps - sum;
		if (sum <= 0) {
			lacking[i] = 0;
			sum = 0;
		}
		else {
			lacking[i] = sum;
		}
		//printf("absent in %i: %i\n", i, sum);
	}
	
	double high = -99999;
	for (i = 0; i < A[me]; i++)
		explore[i] = 0.0;
	int ind, c = 0;
	for (i = 0; i < numHypos; i++) {
		base10tobaseK(i, A[1-me], digits);
		
		if (unique(digits, lacking)) {
			for (j = 0; j < hypothars->numStates; j++) {
				if (!lacking[j]) {
					for (k = 0; k < A[1-me]; k++) {
						hypothars->pi[j][k] = thars->pi[j][k];
						//printf("%.2lf ", hypothars->pi[j][k]);
					}
				}
				else {
					for (k = 0; k < A[1-me]; k++) {
						val = _kappa[j][k] - 1;
						if (k == digits[j])
							val += lacking[j];
						hypothars->pi[j][k] = (double)val / samps;//(samps - A[1-me]);
						//printf("%.2lf (%i; %i)\t", hypothars->pi[j][k], val, _kappa[j][k]);
					}
				}
				//printf("\n");				
			}
			//printf("\n");
		}
		hypoares->bestResponse(M, hypothars, 0);
		//printf("Valor = %.3lf\n", hypoares->Valor[estado]);
		//hypoares->printV();
		
		if (hypoares->Valor[estado] > aspiration) {
			if (hypoares->Valor[estado] > high)
				high = hypoares->Valor[estado];
			
			ind = 0;
			for (j = 1; j < A[me]; j++) {
				if (hypoares->V[estado][j] > hypoares->V[estado][ind])
					ind = j;
			}
			explore[ind] += 1.0;
			c++;
		}
	}


	if (c > 0) {
		printf("%i: high = %.3lf; explore (%i): ", me, high, c);
		for (j = 0; j < A[me]; j++) {
			printf("%.2lf  ", explore[j] / c);
		}
	}
	printf("\n");
	
	delete digits;
	delete lacking;
	
	if (c > 0)
		return true;
	
	return false;
}

void omegaPair::base10tobaseK(int num, int K, int *digits) {
	int index = 0, remainder;
	
	while (num != 0) {
		remainder = num % K;
		num = num / K;
		digits[index] = remainder;
		index ++;
	}
	
	while (index < hypothars->numStates) {
		digits[index] = 0;
		index ++;
	}
}

bool omegaPair::unique(int *digits, int *lacking) {
	int i;
	for (i = 0; i < hypothars->numStates; i++) {
		if (!(lacking[i]) && (digits[i] != 0))
			return false;
	}
	return true;
}