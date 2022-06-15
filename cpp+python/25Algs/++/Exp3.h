#ifndef EXP3_H
#define EXP3_H

#include "defs.h"
#include "expertLearner.h"

class Exp3 : public expertLearner {
public:
	Exp3();
	Exp3(int _me, double _T, double _lambda, int _numExperts);
	~Exp3();

    int select(bool *choices);
    void update(double R, int tau);

	int pickExpert();

	int K;
	double g, eta, lambdita;
	int me;

	double *G, *pt;
	int t;
    
    int experto;
    int *S_e;
};


#endif