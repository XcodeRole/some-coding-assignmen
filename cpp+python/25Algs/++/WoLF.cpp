#include "WoLF.h"

WoLF::WoLF() {
    printf("incomplete WoLF constructor\n");
    exit(1);
}

WoLF::WoLF(int _me, double ***_M, int _A[2], int _omega, bool _verbose) {
	int i, j;

    M = _M;
    A[0] = _A[0];
    A[1] = _A[1];

	me = _me;
	verbose = _verbose;
	//exploration = (double)_exploration / 100.0;
	exploration = 0.1;//0.2;
	omega = _omega;

	numStates = A[0] * A[1];
	numJointActions = numStates;
	numStates = (int)pow((double)numStates, (double)omega);

	Q = new double*[numStates];
	pi = new double*[numStates];
	api = new double*[numStates];
	C = new int[numStates];
	cuenta = new int*[numStates];
	for (i = 0; i < numStates; i++) {
		Q[i] = new double[A[me]];
		pi[i] = new double[A[me]];
		api[i] = new double[A[me]];
		C[i] = 0;
		cuenta[i] = new int[A[me]];

		for (j = 0; j < A[me]; j++) {
			Q[i][j] = 0.0;
			pi[i][j] = 1.0 / A[me];
			api[i][j] = 1.0 / A[me];
			cuenta[i][j] = 0;
		}
	}

	estado = 0;
	t = 0;

	stateHistory = new int[numStates];
	for (i = 0; i < numStates; i++)
		stateHistory[i] = -1;
	
	jointActionHistory = new int[omega];
	for (i = 0; i < omega; i++)
		jointActionHistory[i] = -1;

	indices = new int[A[me]];
	for (i = 0; i < A[me]; i++)
		indices[i] = i;

	if (verbose) {
		char nombre[1000];
		sprintf(nombre, "WoLF%i.dat", me);
		fp = fopen(nombre, "w");
	}
}

WoLF::~WoLF() {
	int i;

	if (verbose)
		fclose(fp);

	for (i = 0; i < numStates; i++) {
		delete Q[i];
		delete pi[i];
		delete api[i];
		delete cuenta[i];
	}

	delete indices;
	delete Q;
	delete pi;
	delete api;
	delete cuenta;
	delete C;
}

int WoLF::move() {
	if (((double)rand() / (double)RAND_MAX) < exploration)
		return rand() % A[me];
	return generateAction();
}

int WoLF::generateAction() {
	int num = rand() % 0x7fff, numero, tmp;
	int i;
    
    //for (i = 0; i < A[me]; i++) {
    //    printf("%.3lf ", pi[estado][i]);
    //}

	for (i = 0; i < A[me]; i++) { // mix it up
		numero = rand() % A[me];
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
	for (i = 0; i < A[me]; i++) {
		sum += pi[estado][indices[i]] * 0x7fff;
		if (num < sum) {
			return indices[i];
		}
	}
	printf("i never selected an action: num = %i\n", num);
	printf("indices[0] = %i; indices[1] = %i\n", indices[0], indices[1]);
//	printf("%i: pi[0] = %lf; pi[1] = %lf\n", t, pi[0], pi[1]);
//	dump();
	//getch();
	exit(1);

	return -1;
}

void WoLF::update(int *acts) {
	int sprime = 0, i;
    double r_t = M[me][acts[0]][acts[1]];

	sprime = getNewState(acts);
	
	if ((estado >= 0) && (sprime >= 0)) {
		double Alpha = 1.0 / (10.0 + (t / 100.0));//((double)(cuenta[estado][actions[me]]) / 10000.0));
        //double Alpha = 1.0 / (100.0 + (t / 10000.0));
		Q[estado][acts[me]] = (1.0 - Alpha) * Q[estado][acts[me]] + Alpha * r_t;// + gamma * maxQ(sprime));
		C[estado] ++;
		cuenta[estado][acts[me]] ++;
		
		for (i = 0; i < A[me]; i++) {
			api[estado][i] = api[estado][i] + ((1.0 / (double)(C[estado])) * (pi[estado][i] - api[estado][i]));
		}
		Project(api[estado], api[estado], A[me]);

		for (i = 0; i < A[me]; i++) {
			pi[estado][i] += trisa(i);
		}
		//pi[estado][actions[me]] += trisa(actions[me]);
		Project(pi[estado], pi[estado], A[me]);
	}

	estado = sprime;
	updateStateHistory(estado, acts);

	if (verbose)
		dump(acts);

	// decay exploration rate
	exploration = 1.0 / (10.0 + (t / 100.0));
    //exploration = 0.05;
	t ++;
	//exploration = 0.2 - 0.00006*t;
	//if (exploration < 0.0)
	//	exploration = 0.0;
}

double WoLF::trisa(int a) {
	double val;
	if (Q[estado][a] < (maxQ(estado) - 0.00001)) {
		val = -deltasa(a);
		//printf("%i: (argmax)val = %lf for action %i\n", t, val, a);
	}
	else {
		int i;
		double sum=0.0;
		for (i = 0; i < A[me]; i++) {
			if (i != a)
				sum += deltasa(i);
		}

		val = sum;
		//printf("%i: (suboptimal)val = %lf for action %i\n", t, val, a);
	}
	
	return val;
}

double WoLF::deltasa(int a) {
	int i;
	double sumo = 0.0, suma = 0.0;
	for (i = 0; i < A[me]; i++) {
		sumo += pi[estado][i] * Q[estado][i];
		suma += api[estado][i] * Q[estado][i];
	}

	if (sumo > suma) { // was 20.0
		double d = 1.0 / (100.0 + (double)(cuenta[estado][a] / 100.0));
		//double d = 1.0 / (20000.0 + t);//cuenta[estado][a]);
		return minimum(pi[estado][a], d / (double)(A[me] - 1));
	}
	else {	// was 20.0
		double d = 2.0 / (100.0 + (double)(cuenta[estado][a] / 100.0));
		//double d = 4.0 / (20000.0 + t);//cuenta[estado][a]);
		return minimum(pi[estado][a], d / (double)(A[me] - 1));
	}
}

double WoLF::minimum(double a, double b) {
	if (a > b)
		return b;
	else
		return a;
}

double WoLF::maxQ(int s) {
	int i;
	double max = Q[s][0];
	
	for (i = 1; i < A[me]; i++) {
		if (max < Q[s][i])
			max = Q[s][i];
	}
	return max;
}

void WoLF::Project(double *nx, double *x, int nacts) {
	int i;
	double sum = 0.0;
	for (i = 0; i < nacts; i++) {
		sum += x[i];
	}
	//if (fabs(sum-1.0) > 0.000001)
	//	printf("sum = %lf; %lf %lf %lf\n", sum, x[0], x[1], x[2]);
	
	for (i = 0; i < nacts; i++) {
		nx[i] = x[i] / sum;
	}
	
	/*	
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
	 } while (diff2 != 0.0);*/
}

void WoLF::dump(int acts[2]) {
	if ((t % 100) != 0)
		return;
	int i;
	
	fprintf(fp, "%i:\t", t);
	for (i = 0; i < A[me]; i++) {
		fprintf(fp, "%lf\t", pi[estado][i]);//, Q[estado][i]);
	}
	fprintf(fp, "\n");
}


void WoLF::updateStateHistory(int ns, int acts[2]) {
	int i;

	for (i = 0; i < numStates - 1; i++) 
		stateHistory[i] = stateHistory[i+1];

	stateHistory[numStates-1] = ns;

	int sprime = 0;
	for (i = 0; i < 2; i++) {
		sprime += (int)(pow(A[i], i)) * acts[i];
	}
	for (i = 0; i < omega - 1; i++) {
		jointActionHistory[i] = jointActionHistory[i+1];
	}
	jointActionHistory[omega-1] = sprime;
}

int WoLF::getNewState(int acts[2]) {
	int i, newState = 0;
	int tier = 1;

	if (omega > 0) {
		for (i = 1; i < omega; i++) {
			if (jointActionHistory[i] == -1)
				return -1;

			newState += tier * jointActionHistory[i];
			tier *= numJointActions;
		}

		int sprime = 0;
		for (i = 0; i < 2; i++) {
			sprime += (int)(pow(A[i], i)) * acts[i];
		}

		newState += sprime * tier;
	}

	return newState;
}
