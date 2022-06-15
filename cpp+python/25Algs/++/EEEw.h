#ifndef EEE_H
#define EEE_H

#include "defs.h"
#include "Mala.h"
#include "omegaStrategy.h"
#include "minimaxLog.h"

class EEE : public Mala {
public:
	EEE();
	EEE(int _me, int _A[NUM_PLAYERS], double ***_M);
	~EEE();
	
	void deriveStrategies();
	void minimaxStuff();
	bool isPure(double *v);
	
	int move();
	void update(int *acts);
	
	double ***M;
	int A[NUM_PLAYERS], me;
	
	int numStrats, numStates;
	omegaStrategy **theStrats;	
	
	double *M_e;
	int *N_e, *S_e, n_i;
	int t, count;
	int a;
	int estado;
	double r;
};

#endif