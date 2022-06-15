#ifndef GIGA_WOLF_H
#define GIGA_WOLF_H

#include "defs.h"
#include "Mala.h"

class GIGA_WoLF : public Mala {
public:
	GIGA_WoLF();
	GIGA_WoLF(int _numAgents, int *_numActions, int _me, bool _verbose, double ***_M);
	~GIGA_WoLF();

	int move();
	int generateAction();

    void update(int *acts);
	void Px(double *nx, double *x, int nacts);
	double L2(double *v1, double *v2);
	double minimum(double a, double b);
	void dump(int *actions);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me);
	//void loadPayoffMatrix(double **pm, int _numJointActions, int _me, double _mv, double *_ms);

	void updateStateHistory(int ns, int *actions);
	int getNewState(int *actions);
	void generateRandomVector(double *v, int len);

    double ***M;
	double **zt, **zt1, **xt, **xt1;
	bool verbose;
	//double eta;
	int nAgents, *nActions;
	int *indices;
	int numStates, me, estado, numJointActions;

	//double **Scores;//*rt;

	FILE *fp;

	int *stateHistory, *jointActionHistory;
	int omega;
	
	int tiempo;
};

#endif