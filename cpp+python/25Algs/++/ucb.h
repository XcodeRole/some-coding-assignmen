#ifndef ucb_H
#define ucb_H

#include "defs.h"
#include "expertLearner.h"

class ucb : public expertLearner {
public:
	ucb();
	ucb(int _me, double _lambda, int _numExperts);
	~ucb();

    int select(bool *choices);
    void update(double R, int tau);
    
    int randomlySelect(bool *choices);

	int me;
	double *x;
	int *n, num;
	int t, experto;
};


#endif