#ifndef staticStrat_H
#define staticStrat_H

#include "defs.h"
#include "Mala.h"
#include "minimaxLog.h"

class staticStrat : public Mala {
public:
	staticStrat();
	staticStrat(int _me, int _A[NUM_PLAYERS], double ***_M, char *strStrat);
	~staticStrat();
	
	int move();
	void update(int *acts);

	double ***M;
	int A[NUM_PLAYERS], me;
	
	int numStates;
	int estado;
	
	bool minmax;
	int *actions;

	minimaxLog *mnmx;
};

#endif