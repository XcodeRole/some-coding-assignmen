#include "MQubed.h"

extern void writeMaximinLog(int A[2], double ***M);

MQubed::MQubed() {
}

MQubed::MQubed(int _numAgents, double ***_M, int *_numActions, int _me, double _gamma, double _alpha, int _eta, int _omega, 
				   bool _q0, bool _nomm, bool _nosecurity, bool _eg, bool _alphad, bool _verbose, bool _hsarsa, char _game[1024]) {
	int i, j;
	
    printf("MQubed constructor\n");
    
    M = _M;
    A = _numActions;
    strcpy(game, _game);
    
	nAgents = _numAgents;
	nActions = new int[nAgents];
	for (i = 0; i < nAgents; i++)
		nActions[i] = _numActions[i];

	gamma = _gamma;
	//alpha = _alpha;
	eta = _eta;
	omega = _omega;
	verbose = _verbose;

	q0 = _q0;
	nomm = _nomm;
	nosecurity = _nosecurity;
	eg = _eg;
	alphad = _alphad;
	hsarsa = _hsarsa;

	estado = -1;
	me = _me;
	if (verbose) {
		printf("are we really starting something\n");
		char nombre[1000];
		sprintf(nombre, "mqubed%i.dat", me);
		fp = fopen(nombre, "w");
	}

	numStates = nActions[0];
	for (i = 1; i < nAgents; i++)
		numStates *= nActions[i];
	
	//printf("numStates = %i; %i * %i\n", numStates, nActions[0], nActions[1]);
	
	numJointActions = numStates;

	numStates = (int)pow((double)numStates, (double)omega);

	Q = new double*[numStates];	
	cs = new int *[numStates];
	for (i = 0; i < numStates; i++) {
		Q[i] = new double[nActions[me]];
		cs[i] = new int[nActions[me]];
		for (j = 0; j < nActions[me]; j++) {
			Q[i][j] = 0.0;
			cs[i][j] = 0;
		}
	}

	pi = new double[nActions[me]];
	for (i = 0; i < nActions[me]; i++) {
		pi[i] = 1.0 / (double)(nActions[me]);
	}
	indices = new int[nActions[me]];
	for (i = 0; i < nActions[me]; i++)
		indices[i] = i;
	stateHistory = new int[numStates];
	for (i = 0; i < numStates; i++)
		stateHistory[i] = -1;
	
	jointActionHistory = new int[omega];
	for (i = 0; i < omega; i++)
		jointActionHistory[i] = -1;

	t = 0;

	mvcount = 0;

	mu_t = 0.0;
	Beta_t = 0.0;
	beta_t = rand() % 2;
	theta_t = 0.0;

	prev = 0;
	
	//mnmx = new minimaxLog(nActions[me]);
	
	alwaysPessimistic = false;
	epsilonLoss = 0.05 + ((double)rand() / (double)(RAND_MAX) / 20.0);
	printf("epsilonLoss = %lf\n", epsilonLoss);
	double time2learn = numStates * nActions[me] * 500;
	totalAcceptableLoss = epsilonLoss * time2learn;//(numStates * nActions[me] * (log(0.1) / log(1 - alpha_l + alpha_l * gamma)));
    
    loadPayoffMatrix();
}

MQubed::~MQubed() {
	int i;

	printf("Beta_t = %lf\n", Beta_t);
	//printf("totalAcceptableLoss = %lf\n", totalAcceptableLoss);
	
	if (verbose) {
		int j;
		for (i = 0; i < numStates; i++) {
			fprintf(fp, "%i: ", i);
			for (j = 0; j < nActions[me]; j++) {
				fprintf(fp, "%lf\t", Q[i][j]);
			}
			fprintf(fp, "\n");
		}
		
		fclose(fp);
	}

	for (i = 0; i < numStates; i++) {
		delete Q[i];
		delete cs[i];
	}

	delete mnmx;
	delete stateHistory;
	delete indices;
	delete nActions;
	delete Q;
	delete cs;
	delete pi;
}

int MQubed::move() {
	return generateAction();
}

//void MQubed::Update(int *actions, double r_t) {
void MQubed::update(int *actions) {
    double r_t = M[me][actions[0]][actions[1]];
    
	r_t = (r_t - low) / (high - low); // make sure this value is scaled between 0 and 1
	
	mu_t += r_t;

	int sprime = getNewState(actions);

//	printf("new state = %i\n", sprime);
//	fflush(stdout);
	
	t++;
	if (estado != -1)
		cs[estado][actions[me]] ++;
	
	if (sprime >= 0) {		
		double Lt = t * mnmx->mv - mu_t;
		if (Lt < 0.0)
			Lt = 0.0;
		if (Lt >= totalAcceptableLoss)
			alwaysPessimistic = true;
		
		if (!alwaysPessimistic)
			Beta_t = pow(Lt / totalAcceptableLoss, 10);
		else
			Beta_t = 1.0;
		
		// select a new beta_t
		if (((double)rand() / (double)(RAND_MAX)) < Beta_t) {
			beta_t = 1;
			if (nosecurity)
				theta_t = maxQ(sprime);
			else
				theta_t = -99999;//(mu_t / t) / (1.0 - gamma);
		}
		else {
			beta_t = 0;
			theta_t = maxQ(sprime);
		}
		
		updatePolicy(sprime);

		if (alphad) {
			if (estado >= 0)
				alpha = 1.0 / ((1.0 / alpha_0) + (cs[estado][actions[me]] / 100.0));
			else
				alpha = alpha_0;
		}
		
		
		if (estado >= 0)
			Q[estado][actions[me]] = (1.0 - alpha) * Q[estado][actions[me]] +
										alpha * (r_t + gamma * nextValue(sprime));
		

		updatePolicy(sprime);
	}
	
	estado = sprime;
	updateStateHistory(estado, actions);

	if (verbose) {
		dumpInfo(actions);
	}
}

void MQubed::loadPayoffMatrix() {
	int i, j;

	high = 1.0;
	low = 0.0;

	for (i = 0; i < numStates; i++) {
		for (j = 0; j < nActions[me]; j++) {
			if (q0 && !hsarsa)
				Q[i][j] = ((double)rand() / (double)(RAND_MAX)) / (1.0 - gamma);
			else
				Q[i][j] = high / (1.0 - gamma);
		}
	}
	
	//mnmx->getMinimax(nActions, me, pmatrix);
    mnmx = computeMaximin(me);
	printf("minmax value: %lf (", mnmx->mv);
	for (i = 0; i < nActions[me]; i++) {
		printf("%lf ", mnmx->ms[i]);
	}
	printf("\n");
	
	if (nomm && q0 && eg && alphad) {  // if sarsa
		alpha = alpha_0 = 0.1;
	}
	else {
		alpha = (1 - pow(epsilonLoss/(1-mnmx->mv+epsilonLoss), epsilonLoss*nActions[me]*numStates/totalAcceptableLoss)) / (1.0-gamma);
		alpha_0 = alpha;
	}
	printf("********alpha = %lf\n", alpha);
	printf("epsilonloss = %lf\n", epsilonLoss);
	printf("paren = %lf\n", epsilonLoss/(1-mnmx->mv+epsilonLoss));
	printf("gamma = %lf\n", gamma);
	fflush(stdout);

	
	if (alpha > 1.0) {
		printf("ALPHA TO BIG\n");
		alpha = 0.5;
	}
}


double MQubed::minimum(double a, double b) {
	if (a > b)
		return b;
	else
		return a;
}

double MQubed::maximum(double a, double b) {
	if (a > b)
		return a;
	else
		return b;
}

void MQubed::updatePolicy(int s) {
	int i;

	if (s < 0)
		return;

	if ((theta_t < (mnmx->mv / (1.0 - gamma))) && !nomm) {
		for (i = 0; i < nActions[me]; i++)
			pi[i] = mnmx->ms[i];
	}
	else {
		double max = Q[s][0];
		int index = 0; 
		for (i = 1; i < nActions[me]; i++) {
			if (Q[s][i] > max) {
				max = Q[s][i];
				index = i;
			}
			else if ((Q[s][i] == max) && (rand() % 2)) {
				max = Q[s][i];
				index = i;
			}
		}
		for (i = 0; i < nActions[me]; i++) {
			if (index == i)
				pi[i] = 1.0;
			else
				pi[i] = 0.0;
		}
	}

	// check to see if we are passing through state with global maxQ
	int gmQ = gmaxQ();
	if ((!inHistory(gmQ) && (beta_t < 1.0)) || eg) {
		double etita = (double)eta * (1000.0 / (1000.0 + mostActions(estado)));
		double percent = etita / 100.0;
		for (i = 0; i < nActions[me]; i++) {
			pi[i] = (1.0 - percent) * pi[i] + percent * (1.0 / (double)nActions[me]);
		}
	}
}

int MQubed::mostActions(int estado) {
	if (estado < 0)
		return 0;
	
	int i;
	int max = cs[estado][0];
	for (i = 0; i < nActions[me]; i++) {
		if (max < cs[estado][i])
			max = cs[estado][i];
	}
	
	return max;
}

int MQubed::generateAction() {
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
		sum += pi[indices[i]] * 0x7fff;
		if (num < sum) {
			return indices[i];
		}
	}
	printf("i never selected an action: num = %i\n", num);
	printf("indices[0] = %i; indices[1] = %i\n", indices[0], indices[1]);
	printf("%i: pi[0] = %lf; pi[1] = %lf\n", t, pi[0], pi[1]);
	//getch();
	exit(1);

	return -1;
}

double MQubed::nextValue(int s) {
	return expectedQ(s);
}

double MQubed::expectedQ(int s) {
	double sum = 0.0;
	int i;

	for (i = 0; i < nActions[me]; i++) {
		sum += pi[i] * Q[s][i];
	}

	return sum;
}

double MQubed::maxQ(int s) {
	int i;
	double max = Q[s][0];

	for (i = 1; i < nActions[me]; i++) {
		if (max < Q[s][i])
			max = Q[s][i];
	}

	return max;
}

int MQubed::gmaxQ() {
	double maxmax = maxQ(0);
	int i, index = 0;

	for (i = 1; i < numStates; i++) {
		if (maxQ(i) > maxmax) {
			maxmax = maxQ(i);
			index = i;
		}
	}

	return index;
}

void MQubed::dumpInfo(int *a) {
	if (estado >= 0)
		fprintf(fp, "%i\t%lf\t%lf\t%lf\t%lf\t%lf\n", t, maxQ(gmaxQ()), maxQ(estado), (mu_t / t) / (1.0 - gamma), mnmx->mv / (1.0 - gamma), Beta_t * 4.0);
}

void MQubed::updateStateHistory(int ns, int *actions) {
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

bool MQubed::inHistory(int s) {
	double GMQ = maxQ(s);
	int i;

	double varEpsilon = 0.01 / (1.0 - gamma);
	for (i = 0; i < numStates; i++) {
		if (stateHistory[i] < 0)
			return false;

		if ((maxQ(stateHistory[i]) + varEpsilon) >= GMQ) {
			return true;
		}
	}

	return false;
}

int MQubed::getNewState(int *actions) {
	int i, newState = 0;
	int tier = 1;
	
//	printf("actions: %i, %i\n", actions[0], actions[1]);
//	fflush(stdout);

	for (i = 1; i < omega; i++) {
		if (jointActionHistory[i] == -1)
			return -1;

		newState += tier * jointActionHistory[i];
		tier *= numJointActions;
	}

	int sprime = 0;
	for (i = 0; i < nAgents; i++) {
		sprime += (int)(pow(nActions[1-i], i)) * actions[i]; // assuming just 2 (perceived) players here
	}

	newState += sprime * tier;

	return newState;
}

minimaxLog *MQubed::computeMaximin(int index) {
	int i, j;
    
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
/*
minimaxLog *MQubed::computeMaximin(int index) {
    printf("reading maximin %i\n", index); fflush(stdout);
    
    char fname[1024];
    sprintf(fname, "../results/maximinVals/%s", game);
    FILE *fp = fopen(fname, "r");
    
    if (fp == NULL) {
        printf("Creating %s", fname); fflush(stdout);
        writeMaximinLog(A, M);
        fp = fopen(fname, "r");
    }
    
    int i;
    minimaxLog *mm = new minimaxLog(nActions[index]);
    if (index == 1) {
        double dummy;
        fscanf(fp, "%lf", &dummy);
        for (i = 0; i < nActions[0]; i++)
            fscanf(fp, "%lf", &dummy);
    }
    fscanf(fp, "%lf", &(mm->mv));
    for (i = 0; i < nActions[index]; i++)
        fscanf(fp, "%lf", &(mm->ms[i]));
    
    fclose(fp);
    
    return mm;
}
*/