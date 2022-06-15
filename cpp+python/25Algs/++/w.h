#ifndef W_H
#define W_H

#include "defs.h"
#include "Mala.h"
#include "expertLearner.h"
#include "omegaStrategy.h"
#include "minimaxLog.h"

#include "a.h"
#include "Exp3.h"
#include "eee.h"
#include "ucb.h"
#include "BR.h"

class W : public Mala {
public:
	W();
    W(const char *nombre, int _omega, int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda);
	~W();
	
	int move();
	void update(int *acts);

	void deriveStrategies();
    void deriveAllStrategies(int numMyCombos);
    int deriveRandomStrategies();
    bool findMatch(int index, int **strats);
	void getPotentialPolicies();
	bool isPure(double *v);
	bool isSame(double *u, double *v);
	void computeMaximin();
	void addMaximin();
	int determineNumExperts(int numCombos);
    void resetCycle();
    
	double ***M;
	int A[NUM_PLAYERS], me, omega;
	int t, estado, experto;
    bool *beenThere;
    double R;
    int tau;
    bool cycled;
    
    expertLearner *learner;
    
    int numStates;
	double **myPolicies;//, **hisPolicies;
	int numMyPolicies, numMyExperts;//, numHisPolicies, numHisExperts;
    
	minimaxLog *mnmx;    
	
	omegaStrategy **theStrats;
	omegaStrategy *currentStrategy;
	
    bool *satisficingExperts;
    
    bool fullSet;
    BR *br;
};


#endif
