#ifndef eee_H
#define eee_H

#include "defs.h"
#include "expertLearner.h"

class eee : public expertLearner {
public:
	eee();
	eee(int _me, double _lambda, int _numExperts);
	~eee();

    int select(bool *choices);
    void update(double R, int tau);

    int randomlySelect(bool *choices);
    
	int me;
	double *M_e;
	int *N_e, *S_e;
	int t, experto;
};


#endif