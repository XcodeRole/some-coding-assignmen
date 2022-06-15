#ifndef ALGOMEGA_H
#define ALGOMEGA_H

#include "defs.h"
#include "Mala.h"
#include "iModel.h"
#include "BR.h"
#include "omegaPair.h"

class algOmega : public Mala {
public:	
	algOmega();
	algOmega(int _me, int _A[NUM_PLAYERS], double ***_M, double _decay);
	~algOmega();
	
	int move();
	void update(int *acts);
	
	void updateImBR();
	
	double ***M;
	int A[NUM_PLAYERS], me;

	iModel *im;
	BR *br;
	
	omegaPair *imBR;
	int **currentImKappa;
	
	double decay;
	double theta;
};

#endif