#ifndef QLearner_H
#define QLearner_H

#include "defs.h"
#include "Mala.h"

class QLearner : public Mala {
public:
	QLearner();
	QLearner(int _me, double ***_M, int _A[2], double _gamma, bool _setHigh);
	~QLearner();

	int move();
	void update(int acts[2]);

	int argmaxQ();
	double maxQ(int s);

    double ***M;
    int A[2];
    
	int numStates, estado, me;
	int t;

	bool verbose;
	bool decay;

	double gamma;
	double r_max;
	double **Q;
	int **cuenta;
	
	bool setHigh;
	int numRandomMoves;
};

#endif