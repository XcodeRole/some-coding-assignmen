#include "SAlg.h"

SAlg::SAlg() {
}

SAlg::SAlg(int _nActions, double _lambda, int _me, double ***_M) {
	nActions = _nActions;
	//lambda = _lambda;
	//lambda = 0.99;
	lambda = _lambda;//(950.0 + (rand() % 41)) / 1000.0;
	me = _me;
    M = _M;
	
	//lambda += (1.0 - lambda) * .9;
	
	satisficing = false;
	episode = -1;

	r_max = -99999;	
	
    aspiration = 1.0;
	//mnmx = new minimaxLog(_nActions);
}

SAlg::~SAlg() {
}

int SAlg::move() {
	episode ++;
	
	if (satisficing)
		return prevAction;
	else {
		double frac = (double)(rand()) / (double)(RAND_MAX);
		frac *= (double)nActions;
		int a = (int)frac;
		//int a = (int)(((double)(rand()) / (double)(RAND_MAX)) * nActions);
		if (a == nActions)
			a--;
		//int a = rand() % nActions;
		prevAction = a;
		return a;
	}
}

void SAlg::update(int *acts) {
    double r_t = M[me][acts[0]][acts[1]];
    
	if (r_t >= aspiration)
		satisficing = true;
	else
		satisficing = false;

	aspiration = ((1.0 - lambda) * r_t) + (lambda * aspiration);
	
	if (r_t > r_max)
		r_max = r_t;
}
/*
void SAlg::loadPayoffMatrix(double **pm, int _numJointActions, int _me) {
	int i;

	double high = -9999999;
	for (i = 0; i < _numJointActions; i++) {
		if (high < pm[i][me])
			high = pm[i][me];
	}
	
	setAspiration(high);
}

void SAlg::loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms) {
	int i;
	
	double high = -9999999;
	for (i = 0; i < _numJointActions; i++) {
		if (high < pm[i][me])
			high = pm[i][me];
	}
	
	setAspiration(high);
	
	// assume square matrix
	mnmx->mv = _mv;
	int dim = (int)sqrt(_numJointActions);
	for (i = 0; i < dim; i++)
		mnmx->ms[i] = _ms[i];
}

void SAlg::setAspiration(double _val) {
	aspiration = _val;
}
*/