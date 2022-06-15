#include "Exp3_act.h"

Exp3_act::Exp3_act() {
}

Exp3_act::Exp3_act(int _nActions, int _me, double _g, double ***_M) {
	K = _nActions;
	me = _me;
    M = _M;
	
	if (_g > 0.0)
		g = _g * K;
	else
		g = 50000 * K;

	G = new double[K];
	pt = new double[K];

	lambda = sqrt((K * log((double)K))/((2.7183 - 1)*g));
	printf("lambda = %lf\n", lambda);
	eta = lambda / K;

	for (int i = 0; i < K; i++)
		G[i] = 0.0;

	t = 0;
	
	//mnmx = new minimaxLog(K);
}

Exp3_act::~Exp3_act() {
	delete G;
	delete pt;
}

int Exp3_act::move() {
	// fill pt
	int i;
	double sum = 0.0;
	//double one = exp(eta * G[0]);
	//double two = exp(eta * G[1]);
	for (i = 0; i < K; i++) {
		sum += exp(eta * G[i]);
	}

	double lambda = 1.0 / (10 + (t/100.0));
	for (i = 0; i < K; i++) {
		pt[i] = (1.0 - lambda) * (exp(eta * G[i]) / sum) + (lambda / K);
	}

	t ++;

	return generateAction();
}

int Exp3_act::generateAction() {
	int num = rand() % RAND_MAX;
	int i;

	double sum = 0.0;
	for (i = 0; i < K; i++) {
		sum += pt[i] * RAND_MAX;
		if (num <= sum) {
			return i;
		}
	}
	printf("i never selected an action: num = %i\n", num);
	char ch;
	//scanf("press a key...", &ch);
	//printf("\n");
	//getch();
	exit(1);

	return -1;
}

void Exp3_act::update(int *acts) {
    G[acts[me]] += M[me][acts[0]][acts[1]] / pt[acts[me]];
}

void Exp3_act::Update(int *actions, double r_t) {
	G[actions[me]] += r_t / pt[actions[me]];
}