#include "HedgeWMA.h"

HedgeWMA::HedgeWMA() {
}

HedgeWMA::HedgeWMA(int _nActions, int _me, double _beta, bool _verbose, double ***_M) {
	int i, j;
	
    M = _M;
	K = _nActions;
	me = _me;
	beta = _beta;
	
	w = new double[K];
	pt = new double[K];
	for (i = 0; i < K; i++) {
		pt[i] = 1.0 / K;
		w[i] = 1.0 / K;
	}
	
	t = 0;
    
    verbose = _verbose;
/*
	Scores = new double*[K];
	for (i = 0; i < K; i++) {
		Scores[i] = new double[K];
		for (j = 0; j < K; j++) {
			Scores[i][j] = -99999;
		}
	}
	highval = -99999;
	lowval = 99999;
*/
/*	verbose = _verbose;
	if (verbose) {
		char nombre[1000];
		sprintf(nombre, "wma%i.dat\0", me);
		fp = fopen(nombre, "w");		
	}
	
	mnmx = new minimaxLog(_nActions);*/
}

HedgeWMA::~HedgeWMA() {
    //printf("in delete ... "); fflush(stdout);
	delete w;
    //printf("a ... "); fflush(stdout);
	delete pt;
    //printf("b\n"); fflush(stdout);
	//for (int i = 0; i < K; i++) {
	//	delete Scores[i];
	//}
	//delete Scores;
	
	if (verbose)
		fclose(fp);
}

int HedgeWMA::move() {
	int i;
	double sum = 0.0;
	for (i = 0; i < K; i++) {
		sum += w[i];
	}
	
	//double lambda = 1.0 / (10 + (t/100.0));
	for (i = 0; i < K; i++) {
		pt[i] = w[i] / sum;
	}
	
	t++;

	return generateAction();
}

int HedgeWMA::generateAction() {
	double num = (double)rand() / (double)RAND_MAX;
	int i;
	
	double sum = 0.0;
	for (i = 0; i < K; i++) {
		sum += pt[i];
		if (num <= sum) {
			return i;
		}
	}
	printf("i never selected an action: num = %lf\n", num);
	printf("time = %i; %lf %lf\n", t, pt[0], pt[1]);
	//getch();
	exit(1);
	
	return -1;
}

void HedgeWMA::update(int *acts) {
	int i;
	
	for (i = 0; i < K; i++) {
		//w[i] *= pow(beta,1 - Scores[i][actions[1-me]]);
        if (me == 0)
            w[i] *= pow(beta,1 - M[me][i][acts[1]]);
        else
            w[i] *= pow(beta,1 - M[me][acts[0]][i]);
	}
	
	t++;
	
	//if (verbose) {
	//	dump(actions[me]);
	//}
}
/*
void HedgeWMA::loadPayoffMatrix(double **pm, int _numJointActions, int _me) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < K; i++) {
		for (j = 0; j < K; j++) {
			if (_me == 1) {
				Scores[i][j] = pm[count][_me];
				if (highval < Scores[i][j])
					highval = Scores[i][j];
				if (lowval > Scores[i][j])
					lowval = Scores[i][j];	
			}
			else {
				Scores[j][i] = pm[count][_me];
				if (highval < Scores[j][i])
					highval = Scores[j][i];
				if (lowval > Scores[j][i])
					lowval = Scores[j][i];	
			}
			count ++;
		}
	}
	
	for (i = 0; i < K; i++) {
		for (j = 0; j < K; j++) {
			Scores[i][j] = (Scores[i][j] - lowval) / (highval - lowval);
		}
	}
}

void HedgeWMA::loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < K; i++) {
		for (j = 0; j < K; j++) {
			if (_me == 1) {
				Scores[i][j] = pm[count][_me];
				if (highval < Scores[i][j])
					highval = Scores[i][j];
				if (lowval > Scores[i][j])
					lowval = Scores[i][j];	
			}
			else {
				Scores[j][i] = pm[count][_me];
				if (highval < Scores[j][i])
					highval = Scores[j][i];
				if (lowval > Scores[j][i])
					lowval = Scores[j][i];	
			}
			count ++;
		}
	}
	
	for (i = 0; i < K; i++) {
		for (j = 0; j < K; j++) {
			Scores[i][j] = (Scores[i][j] - lowval) / (highval - lowval);
		}
	}
	
	// assume square matrix
	mnmx->mv = _mv;
	int dim = (int)sqrt(_numJointActions);
	for (i = 0; i < dim; i++)
		mnmx->ms[i] = _ms[i];
}
*/
void HedgeWMA::dump(int a) {
	fprintf(fp, "%i (%i): ", t, a);
	for (int i = 0; i < K; i++) {
		fprintf(fp, "%lf ", w[i]);
	}
	fprintf(fp, "\n");
}