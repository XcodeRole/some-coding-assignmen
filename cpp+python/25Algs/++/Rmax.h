#ifndef RMAX_H
#define RMAX_H

#include "defs.h"
#include "Mala.h"

class Rmax : public Mala {
public:
	Rmax();
	Rmax(int _me, int _A[NUM_PLAYERS], double ***_M, int _omega, int _K, double _gamma);
	~Rmax();
	
	int move();
    int moveGreedy();
	void update(int *acts);
	
	int selectAction(int s);
	void printV();
	
	int updateCurrentHistory(int _actions[NUM_PLAYERS]);
	int state(int **hist, int a0, int a1);
	int stateFromHistory(int **hist);
	int getJointAction(int num, int index);
	
	void bestResponse(double ***M, double **opi);
	double greedyValueIteration(double ***M, double **opi);
	double maxV(int s);
	int argmax(int s);
	void initOptimisticState();
	
	double ***M;
	int A[NUM_PLAYERS], me;
	
	int omega;
	double gamma;
	double **values;
	int ***kappa;
	int numJointActions, numStates;
	int ***stateHistories, **currentHistory, **dummyHist;
	double **pi, **opi;
	int K;
	
	int t, estado;
};

#endif