#ifndef FP_H
#define FP_H

#include "defs.h"
#include "Mala.h"

class FP : public Mala {
public:
	FP();
	FP(int _nActions, int _priorK, int _me, int _omega, double ***_M);
	~FP();

	int move();
    void update(int *acts);
	void Update(int *actions, double r_t);

	int bestResponse();

	void updateStateHistory(int ns, int *actions);
	int getNewState(int *actions);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms);

    double ***M;
	//double **payoffs;
	int **K;
	int nActions;
	int me;
	int estado, t;
	int numStates, numJointActions;

	int *stateHistory, *jointActionHistory;
	int omega;
};

#endif