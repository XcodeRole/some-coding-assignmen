#ifndef EXPERTLEARNER_H
#define EXPERTLEARNER_H

#include "defs.h"

class expertLearner {
public:
	expertLearner();
	virtual ~expertLearner();
	
	virtual int select(bool *choices);
	virtual void update(double R, int tau);
    
    double computeERegret(double ave);
    
    
    double aspiration, lambda;
    
    int numExperts;
    double *x_phi;
    int *cuenta;
};

#endif
