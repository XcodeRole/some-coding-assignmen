#include "gigawolf.h"

GIGA_WoLF::GIGA_WoLF() {
}

GIGA_WoLF::GIGA_WoLF(int _numAgents, int *_numActions, int _me, bool _verbose, double ***_M) {
	int i, j;
	
	omega = 0;
    M = _M;
	printf("in gigawolf constructor\n");

	nAgents = _numAgents;
	nActions = new int[nAgents];
	for (i = 0; i < nAgents; i++) {
		nActions[i] = _numActions[i];
	}

	me = _me;
	//eta = _eta;  // note this value is entered as alpha in main
	verbose = _verbose;
	
	numStates = 1;//nActions[0] * nActions[1];  //2-player symmetric assumption
	numJointActions = numStates;
	numStates = (int)pow((double)numStates, (double)omega);
	estado = 0;//-1;

	xt = new double*[numStates];
	zt = new double*[numStates];
	xt1 = new double*[numStates];
	zt1 = new double*[numStates];
	for (i = 0; i < numStates; i++) {
		xt[i] = new double[nActions[me]];
		zt[i] = new double[nActions[me]];
		xt1[i] = new double[nActions[me]];
		zt1[i] = new double[nActions[me]];

		generateRandomVector(xt[i], nActions[me]);

		for (j = 0; j < nActions[me]; j++) {
			zt[i][j] = xt1[i][j] = zt1[i][j] = xt[i][j];
		}
	}
/*
	Scores = new double*[nActions[0]];
	for (i = 0; i < nActions[0]; i++) {
		Scores[i] = new double[nActions[1]];
		for (j = 0; j < nActions[1]; j++) {
			Scores[i][j] = -99999;
		}
	}
*/
	indices = new int[nActions[me]];
	for (i = 0; i < nActions[me]; i++)
		indices[i] = i;
/*
	if (verbose) {
		char nombre[1000];
		sprintf(nombre, "GIGA_WoLF%i.dat\0", me);
		fp = fopen(nombre, "w");
	}
*/
	stateHistory = new int[numStates];
	for (i = 0; i < numStates; i++)
		stateHistory[i] = -1;
	
	jointActionHistory = new int[omega];
	for (i = 0; i < omega; i++)
		jointActionHistory[i] = -1;
	
	tiempo = 0;
	
	//mnmx = new minimaxLog(nActions[me]);
}

GIGA_WoLF::~GIGA_WoLF() {
	if (verbose)
		fclose(fp);

	int i;
	for (i = 0; i < numStates; i++) {
		delete xt[i];
		delete zt[i];
		delete xt1[i];
		delete zt1[i];
	}
	//for (i = 0; i < nActions[0]; i++) {
	//	delete Scores[i];
	//}
	//delete Scores;
	//delete rt;
	
	delete xt;
	delete zt;
	delete zt1;
	delete xt1;
	delete nActions;
	delete indices;
}

int GIGA_WoLF::move() {
	if (estado < 0)
		return rand() % nActions[me];
	return generateAction();
}

int GIGA_WoLF::generateAction() {
	int num = rand() % 0x7fff, numero, tmp;
	int i;

	for (i = 0; i < nActions[me]; i++) { // mix it up
		numero = rand() % nActions[me];
		tmp = indices[i];
		indices[i] = indices[numero];
		indices[numero] = tmp;
	}

	if (indices[0] == indices[1]) {
		printf("got a indices problem\n");
		//getch();
		exit(1);
	}

	double sum = 0.0;
	for (i = 0; i < nActions[me]; i++) {
		sum += xt[estado][indices[i]] * 0x7fff;
		if (num < sum) {
			return indices[i];
		}
	}
	printf("i never selected an action: num = %i\n", num);
	printf("indices[0] = %i; indices[1] = %i\n", indices[0], indices[1]);
//	printf("%i: %lf, %lf\n", tiempo, xt[0], xt[1]);
//	printf("%i: pi[0] = %lf; pi[1] = %lf\n", t, pi[0], pi[1]);
//	dump();
	//getch();
	exit(1);

	return -1;
}

void GIGA_WoLF::update(int *acts) {
	tiempo ++;
	
	//printf("(%i, %i, %.4lf) ", actions[0], actions[1], Scores[actions[0]][actions[1]]);
	
	double *incr = new double[nActions[me]];
	int i, j;
	
	if (estado >= 0) {
		double eta = 1 / sqrt((double)tiempo);
		for (i = 0; i < nActions[me]; i++) {
			incr[i] = eta * M[me][i][acts[1-me]] + xt[estado][i];
			//incr[i] = eta * rt[i] + xt[estado][i];
		}
		
		Px(xt1[estado], incr, nActions[me]);

		for (i = 0; i < nActions[me]; i++) {
			incr[i] = (eta / 3.0) * M[me][i][acts[1-me]] + zt[estado][i];
			//incr[i] = (eta / 3.0) * rt[i] + zt[estado][i];
		}
		Px(zt1[estado], incr, nActions[me]);

		double delta;
		double dz = L2(zt1[estado], zt[estado]);
		double dx = L2(zt1[estado], xt1[estado]);
		delta = minimum(1.0, dz / dx);

		for (i = 0; i < nActions[me]; i++) {
			xt[estado][i] = xt1[estado][i] + delta * (zt1[estado][i] - xt1[estado][i]);
			zt[estado][i] = zt1[estado][i];
		}
	}

	//if (verbose)
	//	dump(actions);
	
	delete[] incr;
}

void GIGA_WoLF::Px(double *nx, double *x, int nacts) {
	int i;
	double sum = 0.0;
	bool neg = false;
	for (i = 0; i < nacts; i++) {
		nx[i] = x[i];
		sum += nx[i];
		if (x[i] < 0)
			neg = true;
	}
	double diff2 = 0.0;
	double diff = 1.0 - sum;
	
	bool on[10] = {true, true, true, true, true, true, true, true, true, true};
	int tmpc = nacts, c;
	do {
		c = tmpc;
		diff2 = 0.0;
		for (i = 0; i < nacts; i++) {
			if (on[i]) {
				nx[i] += diff / c;
				if (nx[i] <= 0) {
					diff2 += nx[i];
					nx[i] = 0;
					on[i] = false;
					tmpc --;
				}
			}
		}
		diff = diff2;
	} while (diff2 != 0.0);
}

double GIGA_WoLF::L2(double *v1, double *v2) {
	double sum = 0.0;
	int i;

	for (i = 0; i < nActions[me]; i++) {
		sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}

	return sqrt(sum);
}

double GIGA_WoLF::minimum(double a, double b) {
	if (a > b)
		return b;
	else
		return a;
}

void GIGA_WoLF::dump(int *actions) {
	if ((tiempo % 100) != 0)
		return;
	
	int i;
	fprintf(fp, "%i:\t", tiempo);
	for (i = 0; i < nActions[me]; i++)
		fprintf(fp, "%lf\t", xt[0][i]);
	fprintf(fp, "\r\n");
}

void GIGA_WoLF::updateStateHistory(int ns, int *actions) {
	int i;

	for (i = 0; i < numStates - 1; i++) 
		stateHistory[i] = stateHistory[i+1];

	stateHistory[numStates-1] = ns;

	int sprime = 0;
	for (i = 0; i < nAgents; i++) {
		sprime += (int)(pow(nActions[i], i)) * actions[i];
	}
	for (i = 0; i < omega - 1; i++) {
		jointActionHistory[i] = jointActionHistory[i+1];
	}
	jointActionHistory[omega-1] = sprime;
}

int GIGA_WoLF::getNewState(int *actions) {
	int i, newState = 0;
	int tier = 1;

	for (i = 1; i < omega; i++) {
		if (jointActionHistory[i] == -1)
			return -1;

		newState += tier * jointActionHistory[i];
		tier *= numJointActions;
	}

	int sprime = 0;
	for (i = 0; i < nAgents; i++) {
		sprime += (int)(pow(nActions[i], i)) * actions[i];
	}

	newState += sprime * tier;

	return newState;
}

void GIGA_WoLF::generateRandomVector(double *v, int len) {
	int j;
	double mag = 0.0;
	
	for (j = 0; j < len; j++) {
		v[j] = rand();
		mag += v[j];
	}
	
	for (j = 0; j < len; j++) {
		v[j] /= mag;
	}
}
/*
void GIGA_WoLF::loadPayoffMatrix(double **pm, int _numJointActions, int _me) {
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
	
	pmatrix = new double[_numJointActions];
	for (i = 0; i < _numJointActions; i++) {
		pmatrix[i] = pm[i][_me];
	}
	
	// assume square matrix
	int dim = (int)sqrt(_numJointActions);
	int nActs[2];
	nActs[0] = dim;
	nActs[1] = dim;
	
	mnmx->getMinimax(nActs, _me, pmatrix);	
	printf("mv = %lf\n", mnmx->mv);
}


void GIGA_WoLF::loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms) {
	int i, j;
	int s, count = 0;
	
	for (i = 0; i < nActions[1]; i++) {
		for (j = 0; j < nActions[0]; j++) {
			if (_me == 1) {
				Scores[i][j] = pm[count][_me];
			}
			else {
				Scores[j][i] = pm[count][_me];
			}
			count ++;
		}
	}
	
	pmatrix = new double[_numJointActions];
	for (i = 0; i < _numJointActions; i++) {
		pmatrix[i] = pm[i][_me];
	}
	
	// assume square matrix
	mnmx->mv = _mv;
	int dim = (int)sqrt(_numJointActions);
	for (i = 0; i < dim; i++)
		mnmx->ms[i] = _ms[i];
}
*/
