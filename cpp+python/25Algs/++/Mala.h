#ifndef MALA_H
#define MALA_H

#include "defs.h"
#include "expertLearner.h"
#include "omegaStrategy.h"

class Mala {
public:
	Mala();
	virtual ~Mala();
	
	virtual int move();
	virtual void update(int *acts);
    
    //omegaStrategy *currentStrategy;
    
    expertLearner *learner;    
};

#endif