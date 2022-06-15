#ifndef Bully_H
#define Bully_H

#include "defs.h"
#include "Mala.h"
#include "omegaStrategy.h"
#include "omegaPair.h"
#include "minimaxLog.h"
#include "iModel.h"
#include "BR.h"

class Bully : public Mala {
public:
	Bully();
	Bully(int _me, int _A[NUM_PLAYERS], double ***_M);
	~Bully();
	
	void deriveStrategies();
	
	int move();
	void update(int *acts);
	
	double ***M;
	int A[NUM_PLAYERS], me;
	
	int numStrats, numStates;
	omegaPair **theIStrats;	
	int estado, a, t;
};

#endif