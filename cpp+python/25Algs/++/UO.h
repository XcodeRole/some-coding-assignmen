#ifndef UO_H
#define UO_H

#include "defs.h"
#include "Mala.h"
#include "omegaStrategy.h"
#include "minimaxLog.h"

class UO : public Mala {
public:
	UO();
	UO(int _me, int _A[NUM_PLAYERS], double ***_M);
	~UO();
	
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
	bool init;
	int estado;
	
	int a;
	double r;
	int count;
	int t;
	
	bool *beenThere;
};

#endif