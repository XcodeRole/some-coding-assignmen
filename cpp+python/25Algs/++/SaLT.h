#ifndef SALT_H
#define SALT_H

#include "defs.h"
#include "Mala.h"

class SaLT : public Mala {
public:
	SaLT();
	SaLT(int _me, int _A[NUM_PLAYERS], double ***_M, double _lambda);
	~SaLT();
	
	int move();
	void update(int *acts);
	
	double ***M;
	int A[NUM_PLAYERS], me;
	double lambda;
	
	double aspiration;
	double r;
	int a;
};

#endif