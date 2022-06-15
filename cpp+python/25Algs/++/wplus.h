#ifndef WPLUS_H
#define WPLUS_H

#include "defs.h"
#include "Mala.h"
#include "expertLearner.h"
#include "omegaStrategy.h"
#include "omegaPair.h"
#include "iModel.h"
#include "BR.h"
#include "minimaxLog.h"

#include "a.h"
#include "Exp3.h"
#include "eee.h"
#include "ucb.h"

class Wplus : public Mala {
public:
	Wplus();
    Wplus(const char *nombre, int _omega, int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda);
	~Wplus();
	
	int move();
	void update(int *acts);

	void deriveStrategies();
    void deriveAllStrategies();
    //int deriveRandomStrategies();
    //bool findMatch(int index, int **strats);
    
	void getPotentialPolicies();
	bool isPure(double *v);
	bool isSame(double *u, double *v);
	void computeMaximin();
	void addMaximin();
	int determineNumExperts(int numCombos);
    void resetCycle();
    
    int setAspiration0();
    int findSteadyState(omegaPair *oPair, int s);
    void determineSatisficingExperts();
    
    int whichOneIsIt(omegaStrategy *strat);
    bool override();
    void determineSatisficing();
    
    double NB();
    
	double ***M;
	int A[NUM_PLAYERS], me, omega;
	int t, estado, experto, minimaxStrat;
    bool *beenThere;
    double R;
    double lambda;
    int tau;
    bool cycled;
    
    expertLearner *learner;
    
    int numStates;
	double **myPolicies, **hisPolicies;
	int numMyPolicies, numMyExperts, numHisPolicies;
	minimaxLog *mnmx;//, *mnmx2;    
	
	omegaStrategy **theStrats;
	//omegaStrategy *currentStrategy;
	        
	int numTeachingStrats, numIStrats;
	omegaPair **theTeachingStrats, **theIStrats;
	
	iModel *im;
	BR *br;	
	
	bool alwaysMM;
	//omegaStrategy *minimaxStrat;
	double permissibleLoss, mu;
	
	int numviable;
    bool *satisficingExperts;
    bool beSelective;
    
    //bool fullSet;
};


#endif
