#ifndef RNDM_H
#define RNDM_H

#include "defs.h"
#include "Mala.h"

class Rndm : public Mala {
public:
	Rndm();
	Rndm(int numActs, double *_p);
	~Rndm();
	
	int move();
	void update(int *acts);
	
	double *p;
	int numActs;
};

#endif