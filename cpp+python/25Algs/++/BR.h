#ifndef BR_H
#define BR_H

#include "defs.h"
#include "omegaPair.h"
#include "Mala.h"

class BR : public Mala {
public:
	BR();
	BR(int _me, int _A[NUM_PLAYERS], double ***_M);
	~BR();
	
	int move();
	void update(int *acts);
	
	void print();
	
	double ***M;
	int A[NUM_PLAYERS], me;
	
	omegaPair *myBR;
	int **kappa;
	
	int t, estado;
	
	bool verbose;
};

#endif