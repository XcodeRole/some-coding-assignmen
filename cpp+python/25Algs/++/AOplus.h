#ifndef AOplus_H
#define AOplus_H

#include "defs.h"
#include "omegaPair.h"
#include "omegaStrategy.h"
#include "Rmax.h"
#include "Mala.h"
#include "iModel.h"
#include "BR.h"
#include "minimaxLog.h"

class AOplus : public Mala {
public:
	AOplus();
	AOplus(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda);
	~AOplus();
	
	void deriveStrategies();
	void getPotentialPolicies();
	bool isPure(double *v);
	bool isSame(double *u, double *v);
	void computeMaximin();
	void addMaximin();
	int determineNumStrats(int numCombos);
	int findSteadyState(omegaPair *oPair);
	
	int move();
	void update(int *acts);
	
	void selectStrategy();
	int whichOneIsIt(omegaStrategy *strat);
	
	bool override();
	
	int setAspiration0();
	void resetUtils();
	double computeInertia();
	
	double ***M;
	int A[NUM_PLAYERS], me;
	
	double **policies;
	int numPolicies;
	
	int numStates;
	
	int numTeachingStrats, numIStrats, numStrats;
	omegaPair **theTeachingStrats, **theIStrats;
	omegaStrategy **theStrats;
	bool *selectable;
	
	iModel *im;
	BR *br;	
	
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
	bool alwaysMM;
	omegaStrategy *minimaxStrat;
	double permissibleLoss, mu;
	
	int numviable;
};

#endif