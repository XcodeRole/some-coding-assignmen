#ifndef UCB1_H
#define UCB1_H

#include "defs.h"
#include "Mala.h"
#include "omegaStrategy.h"
#include "minimaxLog.h"

class UCB1 : public Mala {
public:
	UCB1();
	UCB1(int _me, int _A[NUM_PLAYERS], double ***_M);
	~UCB1();
	
	void deriveStrategies();
	void minimaxStuff();
	bool isPure(double *v);
	
	int move();
	void update(int *acts);
	
	double ***M;
	int A[NUM_PLAYERS], me;
	
	int numStrats, numStates;
	omegaStrategy **theStrats;	
	
	double *x;
	int *n, num;
	int estado;
	
	int a;
	int count;
	int t;
};

#endif