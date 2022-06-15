#ifndef UOplus_H
#define UOplus_H

#include "defs.h"
#include "Mala.h"
#include "omegaStrategy.h"
#include "omegaPair.h"
#include "minimaxLog.h"
#include "iModel.h"
#include "BR.h"

class UOplus : public Mala {
public:
	UOplus();
	UOplus(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda, bool _satSelector);
	~UOplus();
	
	void deriveStrategies();
	void minimaxStuff();
	bool isPure(double *v);
	bool isSame(double *u, double *v);
	
	int move();
	void update(int *acts);
	
	int tryUntried(int numLeft);
	
	void setAspiration0();
	void findSatisficing();
	bool alreadyAdded(omegaPair *istrat, omegaPair **satStrats, int numviable);
	int whichOneIsIt(omegaStrategy *strat);
	
	double ***M;
	int A[NUM_PLAYERS], me;
	double lambda;
	
	int numStrats, numStates;
	int numTeachingStrats, numIStrats;
	omegaStrategy **theStrats;
	bool satSelector;
	
	omegaPair **theTeachingStrats, **theIStrats;
	
	iModel *im;
	BR *br;		
	
	minimaxLog *mnmx;
	bool alwaysMM;
	omegaStrategy *minimaxStrat;
	double permissibleLoss, mu;
	int mmIndex;
	
	double *x;
	int *n, num;
	int estado;
	
	int a;
	double r;
	int count;
	int t;
	
	bool *beenThere;
	
	double aspiration;
	bool *selectable;
	
	bool *left;
};

#endif