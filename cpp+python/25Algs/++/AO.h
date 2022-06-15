#ifndef AO_H
#define AO_H

#include "defs.h"
#include "omegaPair.h"
#include "omegaStrategy.h"
#include "Rmax.h"
#include "Mala.h"
#include "minimaxLog.h"

class AO : public Mala {
public:
	AO();
	AO(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda);
	~AO();
	
	void deriveStrategies();
	void getPotentialPolicies();
	bool isPure(double *v);
	bool isSame(double *u, double *v);
	void computeMaximin();
	void addMaximin();
	int determineNumStrats(int numCombos);
	
	int move();
	void update(int *acts);
	
	int selectStrategy();
	void setAspiration0();
	void resetUtils();
	double computeInertia();
	
	double ***M;
	int A[NUM_PLAYERS], me;
	
	double **policies;
	int numPolicies;
	
	int numStrats, numStates;
	omegaStrategy **theStrats;
	
	int t;
	double aspiration, currentValue;
	double lambda;
	int estado;
	omegaStrategy *currentStrategy;
	bool *beenThere, evaluateChange;
	
	int sCuenta;
	double v;
	
	int omega;
	
	minimaxLog *mnmx;
};

#endif