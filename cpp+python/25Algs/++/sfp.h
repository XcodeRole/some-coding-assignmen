#ifndef SFP_H
#define SFP_H

#include "defs.h"
#include "Mala.h"

class sFP : public Mala {
public:
	sFP();
	sFP(int _nActions, int _priorK, int _me, int _omega, bool _verbose, double ***_M);
	void loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms);
	~sFP();
	
	int move();
	void update(int *acts);
	
	int bestResponse();
	int generateAction(double *x);
	double scaled(double val);
	//void dump();
	
	void updateStateHistory(int ns, int *actions);
	int getNewState(int *actions);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me);
	
	//double **payoffs;
    double ***M;
	double *utilities, *pi;
	int **K;
	int nActions;
	//double exploration;
	int me;
	int estado, t;
	int numStates, numJointActions;
	
	int *stateHistory, *jointActionHistory;
	int omega;
	
	double highval, lowval;
	
	//FILE *fp;
	bool verbose;
};

#endif