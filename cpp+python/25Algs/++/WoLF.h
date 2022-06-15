#ifndef WOLF_H
#define WOLF_H

#include "defs.h"
#include "Mala.h"

class WoLF : public Mala {
public:
	WoLF();
	WoLF(int _me, double ***_M, int _A[2], int _omega, bool _verbose);
	~WoLF();

	int move();
	int generateAction();
	void update(int acts[2]);
    
    
	double trisa(int a);
	double deltasa(int a);
	double minimum(double a, double b);
	double maxQ(int s);
	void Project(double *nx, double *x, int nacts);
	void dump(int acts[2]);

	void updateStateHistory(int ns, int acts[2]);
	int getNewState(int acts[2]);

    double ***M;
    int A[2];
    
	//double alpha, gamma;
	double delta_l, delta_w;
	double **Q;
	double **pi, **api;
	int **cuenta;
	int *C;
	int t;

	int numStates, numJointActions;
	int estado;
	int me;
	double exploration;
	int *indices;
	bool verbose;

	FILE *fp;

	int *stateHistory, *jointActionHistory;
	int omega;
};

#endif