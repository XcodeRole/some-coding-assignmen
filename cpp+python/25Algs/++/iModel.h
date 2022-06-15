#ifndef IMODEL_H
#define IMODEL_H

#include "defs.h"
#include "omegaStrategy.h"

class iModel {
public:
	iModel();
	iModel(int _me, int _A[NUM_PLAYERS], int _omega);
	~iModel();
	
	void init();
	void update(int *acts, int estado, int _t);
	void print();
	bool match(omegaStrategy *thars);
    bool match(double **pi);
	
	int me, A[NUM_PLAYERS], omega;
	
	int *lastSeen;
	int *lastTime;
	int numStates;

};

#endif