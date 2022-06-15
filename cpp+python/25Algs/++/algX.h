#ifndef ALGA_H
#define ALGA_H

#include "defs.h"
#include "omegaPair.h"
#include "omegaStrategy.h"
#include "iModel.h"
#include "Mala.h"

class algX : public Mala {
public:
	algX();
	algX(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda);
	~algX();
	
	void deriveStrategyPairs();
	
	int move();
	void update(int *acts);
	
	int selectViableStrategy();
	int selectViableTeacher();
	void setAspiration0(int _estado);
	void findViableStrategies();
	void resetUtils(int ind);
	double computeInertia();
	
	double ***M;
	int A[NUM_PLAYERS], me;
	
	int numIModels, numTeachers;
	omegaPair *maximin, **myBRs, **myTeachers;
	int **kappa;
	
	iModel *im;
	
	int t;
	double aspiration, currentValue;
	double lambda;
	int estado;
	omegaPair *currentStrategy;
	double *explore;
	
	int sCuenta, desiredTarget, startState;
	double v;
	
	//double R_teach;
	
	int viableTeacherStrats[1000], viableFollowerStrats[1000];
	int numViableTeachers, numViableFollowers;
	
	int switch2BRcount;
	
	bool verbose;
};

#endif