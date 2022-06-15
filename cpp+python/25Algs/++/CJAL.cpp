#include "CJAL.h"

CJAL::CJAL() {
}

CJAL::CJAL(int _numAgents, int *_numActions, double ***_M, int _me, int _N, double _explor, bool _verbose) {
	int i, j;
    
    M = _M;
	
	nAgents = _numAgents;
	nActions = new int[nAgents];
	for (i = 0; i < nAgents; i++)
		nActions[i] = _numActions[i];
	
	me = _me;
	N = _N;
	explor = _explor;
/*
	Scores = new double*[nActions[0]];
	for (i = 0; i < nActions[0]; i++) {
		Scores[i] = new double[nActions[1]];
		for (j = 0; j < nActions[1]; j++) {
			Scores[i][j] = -99999;
		}
	}
*/
	counts = new int*[nActions[me]];
	for (i = 0; i < nActions[me]; i++) {
		counts[i] = new int[nActions[1-me]];
		for (j = 0; j < nActions[1-me]; j++)
			counts[i][j] = 0;
	}
	
	pi = new double[nActions[me]];
	for (i = 0; i < nActions[me]; i++) {
		pi[i] = 1.0 / nActions[me];
		//printf("pi[%i] = %lf\n", i, pi[i]);
	}
	
	t = 0;
	
	verbose = _verbose;
/*	if (verbose) {
		char nombre[1000];
		sprintf(nombre, "CJAL%i.dat\0", me);
		fp = fopen(nombre, "w");
	}	
*/	
	//mnmx = new minimaxLog(nActions[me]);
}

CJAL::~CJAL() {
	int i;
	//for (i = 0; i < nActions[0]; i++) {
	//	delete Scores[i];
	//}
	//delete Scores;
	for (i = 0; i < nActions[me]; i++) {
		delete counts[i];
	}
	delete counts;
	delete pi;
	delete nActions;
}


int CJAL::move() {
	if (t < N) {
		//printf("pi = %lf %lf\n", pi[0], pi[1]);
		return generateAction();
	}
	if (t >= N) {  // otherwise just proceed with random behavior
		if (((double)rand() / (double)RAND_MAX) < explor) {
			int i;
			for (i = 0; i < nActions[me]; i++)
				pi[i] = 1.0 / nActions[me];
			
			return generateAction();
		}
		else {
			// generate utilities
			int i, j;
			double sum;
			for (i = 0; i < nActions[me]; i++) {
				sum = 0.0;
				pi[i] = 0.0;
				for (j = 0; j < nActions[1-me]; j++) {
					sum += counts[i][j];
				}
				for (j = 0; j < nActions[1-me]; j++) {
					//pi[i] += Scores[i][j] * (counts[i][j] / sum);
                    if (me == 0)
                        pi[i] += M[me][i][j] * (counts[i][j] / sum);
                    else
                        pi[i] += M[me][j][i] * (counts[i][j] / sum);
				}
			}
	
			int highindex = 0;
			
			for (i = 1; i < nActions[me]; i++) {
				if (pi[i] > pi[highindex])
					highindex = i;
				if ((pi[i] == pi[highindex]) && (rand() % 2))
					highindex = i;
			}
			return highindex;
		}
	}
    
    printf("CJAL: problemo\n");
    exit(1);
    
    return -1;
}

int CJAL::generateAction() {
	int num = rand() % RAND_MAX;
	int i;
	
	double sum = 0.0;
	for (i = 0; i < nActions[me]; i++) {
		sum += pi[i] * RAND_MAX;
		if (num <= sum) {
			return i;
		}
	}
	printf("i never selected an action: num = %i\n", num);
	printf("time = %i; %lf %lf\n", t, pi[0], pi[1]);
	//getch();
	exit(1);
	
	return -1;	
}
/*
void CJAL::loadPayoffMatrix(double **pm, int _numJointActions, int _me) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < nActions[0]; i++) {
		for (j = 0; j < nActions[1]; j++) {
			if (_me == 1) {
				Scores[i][j] = pm[count][_me];
			}
			else {
				Scores[j][i] = pm[count][_me];
			}
			count ++;
		}
	}	
}

void CJAL::loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < nActions[0]; i++) {
		for (j = 0; j < nActions[1]; j++) {
			if (_me == 1) {
				Scores[i][j] = pm[count][_me];
			}
			else {
				Scores[j][i] = pm[count][_me];
			}
			count ++;
		}
	}	
	
	// assume square matrix
	mnmx->mv = _mv;
	int dim = (int)sqrt(_numJointActions);
	for (i = 0; i < dim; i++)
		mnmx->ms[i] = _ms[i];
}
*/
void CJAL::update(int *acts) {
    //printf("(c"); fflush(stdout);
    Update(acts, M[me][acts[0]][acts[1]]);
    //printf(")"); fflush(stdout);
}

void CJAL::Update(int *actions, double r_t) {
    //printf("j"); fflush(stdout);

	counts[actions[me]][actions[1-me]] ++;
	t ++;

    //printf("a"); fflush(stdout);
	
	if (verbose) {
		dump();
	}
    //printf("l"); fflush(stdout);
}

void CJAL::dump() {
	int i, j;
	
	fprintf(fp, "%i: ", t);
	for (i = 0; i < nActions[me]; i++) {
		fprintf(fp, "%lf\t", pi[i]);
	}
	fprintf(fp, "\n");
	for (i = 0; i < nActions[me]; i++) {
		for (j = 0; j < nActions[1-me]; j++) {
			fprintf(fp, "%i ", counts[i][j]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
}
